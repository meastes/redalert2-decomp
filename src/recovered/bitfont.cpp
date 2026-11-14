// bitfont.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/bitfont.json carries the addresses and the rows that
// stay in their own files.


#define RA2_BITFONT_MISSING_GLYPH
#include "shared/text_print.hpp"
#include "shared/win32_api.hpp"
#include "shared/raw_file_class.hpp"
#include "shared/cc_file_class.hpp"
#include "shared/crt_memory.hpp"
#define BLIT_CLIPPED(mask) \
    if (cx >= clipL && cx <= clipR && (bits & (mask))) *d = color; \
    d++; cx++; w--;
#define BLIT_PLAIN(mask) \
    if (bits & (mask)) *d = color; \
    d++; w--;
#include "shared/hicolor.hpp"
#include "shared/rgb_globals.hpp"
#include "shared/hicolor_pixel.hpp"
#include "shared/surface.hpp"


class Leaf_431020 { public: void get(); };
void Leaf_431020::get() { unsigned int self = (unsigned int)this; unsigned int zero; unsigned int spare; zero = 0; *(unsigned int*)(0x84e9c0) = zero; *(unsigned int*)(0x84e9c4) = zero; *(unsigned int*)(0x84e9c8) = zero; *(unsigned int*)(0x84e9cc) = zero; }


// Missing_Glyph 0x00431F50 is declared for this unit (Draw_Char 0x004318E0 expands it).
// BitFontClass::BitFontClass(char const *filename) (0x00431040)
// Starts from the defaults, then takes the font file's glyph data and
// metrics (the defaults again when it cannot be loaded).

inline void BitFontClass::Reset()
{
    Data = 0;
    Color = 0x7FFF;
    unknown_26 = 0x3555;
    unknown_28 = 0x40;
    unknown_40 = 1;
    Clipping = 1;
    Width = 1;
    Height = 1;
    unknown_20 = 0;
    Buffer = 0;
    unknown_08 = 0;
    SetRect((RECT *)&ClipBox, 0, 0, 0, 0);
    unknown_2c = 1;
}

// Never expanded inline in its unit (BitTextClass calls it).
#pragma auto_inline(off)
BitFontClass::BitFontClass(char const *filename)
{
    Reset();
    if (!Load_Font(filename)) {
        Reset();
    }
}
#pragma auto_inline(on)


// BitFontClass::~BitFontClass() (0x00431100)
// Releases the cursor glyph copy and the glyph data.

BitFontClass::~BitFontClass()
{
    if (unknown_08) {
        free(unknown_08);
    }
    if (Data) {
        Free_Font_Data(Data);
    }
}


// FontDataType *BitFontClass::Load_Font_Data(char const *filename) (0x00431150)
// Reads a font file's glyph data: a "fonT" file holds the metrics and the character map
// directly, a "FoNt" file a header, a table of character ranges and the glyphs. Null when
// the file cannot be read.

// The header of a "fonT" file (0x1C bytes): the metrics of FontDataType.
struct FontShortHeaderType {
    int Magic;          // 'fonT'
    int unknown_00;
    int Width;
    int unknown_08;
    int Height;
    int GlyphCount;
    int GlyphSize;
};

FontDataType *__fastcall BitFontClass::Load_Font_Data(char const *filename)
{
    FontDataType *data = 0;
    FontShortHeaderType header;
    CCFileClass *file = new CCFileClass(filename);
    if (file != 0) {
        if (file->Is_Available() && file->Open(FILE_RIGHTS_READ)) {
            data = new FontDataType;
            if (data != 0) {
                memset(data, 0, sizeof(FontDataType));
                data->Map = (unsigned short *)operator new(0x10000 * sizeof(unsigned short));
                if (data->Map != 0) {
                    memset(data->Map, 0, 0x10000 * sizeof(unsigned short));
                    if (file->Read(&header, sizeof(header)) == sizeof(header)) {
                        if (header.Magic == 0x546E6F66) {
                            data->unknown_08 = header.unknown_08;
                            data->Height = header.Height;
                            data->Width = header.Width;
                            data->unknown_00 = header.unknown_00;
                            data->GlyphSize = header.GlyphSize;
                            data->GlyphCount = header.GlyphCount;
                            if (file->Read(data->Map, 0x10000 * sizeof(unsigned short)) != 0x10000 * sizeof(unsigned short)) {
                                goto failed;
                            }
                        } else if (header.Magic == 0x744E6F46) {
                            file->Seek(0, FILE_SEEK_START);
                            FontFileHeaderType fileheader;
                            if (file->Read(&fileheader, sizeof(fileheader)) != sizeof(fileheader)) {
                                goto failed;
                            }
                            data->unknown_08 = fileheader.unknown_0c;
                            data->Height = fileheader.Height;
                            data->Width = fileheader.Width;
                            data->unknown_00 = fileheader.unknown_04;
                            data->GlyphSize = fileheader.GlyphSize;
                            FontRangeType *ranges = (FontRangeType *)malloc(fileheader.RangeCount * sizeof(FontRangeType));
                            if (ranges == 0) {
                                goto failed;
                            }
                            file->Seek(fileheader.RangeOffset, FILE_SEEK_START);
                            if (file->Read(ranges, fileheader.RangeCount * sizeof(FontRangeType)) != fileheader.RangeCount * sizeof(FontRangeType)) {
                                goto failed;
                            }
                            data->GlyphCount = 0;
                            for (unsigned index = 0; index < (unsigned)fileheader.RangeCount; index++) {
                                unsigned short glyph = ranges[index].First;
                                for (int character = ranges[index].Low; character <= ranges[index].High; character++, glyph++) {
                                    data->Map[character] = glyph + 1;
                                    data->GlyphCount++;
                                }
                            }
                            free(ranges);
                            file->Seek(fileheader.GlyphOffset, FILE_SEEK_START);
                        } else {
                            goto failed;
                        }
                        int size = data->GlyphCount * data->GlyphSize;
                        data->MappedCount = 0;
                        for (int character = 0; character < 0x10000; character++) {
                            if (data->Map[character]) {
                                data->MappedCount++;
                            }
                        }
                        data->Glyphs = (unsigned char *)operator new(size);
                        if (data->Glyphs != 0 && file->Read(data->Glyphs, size) == size) {
                            goto loaded;
                        }
                    }
                }
failed:
                if (data->Map) {
                    delete data->Map;
                }
                if (data->Glyphs) {
                    delete data->Glyphs;
                }
                delete data;
                data = 0;
            }
        }
loaded:
        delete file;
    }
    return data;
}


// void BitFontClass::Free_Font_Data(FontDataType *data) (0x004313C0)
// Deletes a glyph data block and its tables.

void __fastcall BitFontClass::Free_Font_Data(FontDataType *data)
{
    if (data->Map) {
        delete data->Map;
    }
    if (data->Glyphs) {
        delete data->Glyphs;
    }
    delete data;
}


// bool BitFontClass::Load_Font(char const *filename) (0x004313F0)
// Takes the font file's glyph data and metrics; false when it cannot be loaded.

bool BitFontClass::Load_Font(char const *filename)
{
    Data = Load_Font_Data(filename);
    if (Data) {
        Width = Data->Width;
        Height = Data->Height;
        Make_Missing_Char();
        return true;
    }
    return false;
}


// unsigned short BitFontClass::Set_Color(unsigned short color) (0x00431430)
// Sets the packed text colour and returns the previous one. Re-plan of the
// accepted FontDrawState::ExchangeFontColor_00431430 (population-32) under
// the class text_print.hpp names.

unsigned short BitFontClass::Set_Color(unsigned short color)
{
    unsigned short previous = Color;
    Color = color;
    return previous;
}


// unsigned short BitFontClass::Set_Shadow_Color(unsigned short value) (0x00431440)
// Stores value in the word after the text colour (+0x26) and returns the colour (+0x24). Unreferenced and
// without an inventory row (between Set_Color 0x00431430 and Set_Clipping 0x00431450).

unsigned short BitFontClass::Set_Shadow_Color(unsigned short value)
{
    unknown_26 = value;
    return Color;
}


// unsigned char BitFontClass::Set_Clipping(unsigned char clip) (0x00431450)
// Sets the raw clipping flag (not normalized to a truth value) and returns
// the previous one. Re-plan of the accepted
// FontDrawState::ExchangeFontClippingFlag_00431450 (population-32).

unsigned char BitFontClass::Set_Clipping(unsigned char clip)
{
    unsigned char previous = Clipping;
    Clipping = clip;
    return previous;
}


// void BitFontClass::Set_Clip_Box(tagRECT const *box) (0x00431460)
// Copies the clip box, or clears it without one.

void BitFontClass::Set_Clip_Box(tagRECT const *box)
{
    if (box != 0) {
        ClipBox.left = box->left;
        ClipBox.top = box->top;
        ClipBox.right = box->right;
        ClipBox.bottom = box->bottom;
        return;
    }
    ClipBox.bottom = 0;
    ClipBox.right = 0;
    ClipBox.top = 0;
    ClipBox.left = 0;
}


// 0x004314a0: function not yet identified
//
// Body: 13 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined1 __thiscall Set_Bold_004314a0(int param_1,undefined1 param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004314a0 {

struct Owner_004314a0 { unsigned char Set_Bold_004314a0(unsigned char bold); };

unsigned char Owner_004314a0::Set_Bold_004314a0(unsigned char bold)
{
    int self = (int)this;
    unsigned char previous;

    previous = (unsigned char)(*(unsigned char *)(self + 0x40));
    *(unsigned char *)(self + 0x40) = bold;
    return previous;
}

}  // namespace seed_004314a0


// bool BitFontClass::Get_Text_Area(wchar_t const *text, int *width, int *height, int max_width) (0x004314B0)
// Measures the text: the widest line and the total height, wrapping at the
// last space (or mid-word) when a line passes max_width (0: no limit). Tabs
// advance to the next tab stop; CR, LF and CR LF end a line.

bool BitFontClass::Get_Text_Area(wchar_t const *text, int *width, int *height, int max_width)
{
    int space_widest = 0;
    if (!Data || !text) {
        if (height) {
            *height = 0;
        }
        if (width) {
            *width = 0;
        }
        return false;
    }

    int widest = 0;
    int line_height = Height;
    int total = Height;
    int x = 0;
    int count = 0;
    wchar_t const *space = 0;
    wchar_t last = 0;
    wchar_t c = *text++;
    while (c) {
        switch (c) {
            case '\n':
            case '\r':
                if (last != '\r') {
                    x = 0;
                    count = 0;
                    total += line_height;
                }
                break;

            case '\t':
                x += unknown_28;
                x -= x % unknown_28;
                break;

            case ' ':
                space = text;
                space_widest = widest;
                // fall through
            default: {
                unsigned char *glyph = Glyph_Data(c);
                if (!glyph) {
                    glyph = (unsigned char *)unknown_08;
                }
                if (glyph) {
                    count++;
                    x += unknown_2c + *glyph;
                    if (max_width && x > max_width) {
                        if (space) {
                            if (count > 1) {
                                text = space;
                                widest = space_widest;
                            }
                        } else {
                            if (count > 1) {
                                x -= unknown_2c + *glyph;
                                text--;
                            }
                            if (x > widest) {
                                widest = x;
                            }
                        }
                        x = 0;
                        count = 0;
                        total += line_height;
                        space = 0;
                    } else if (x > widest) {
                        widest = x;
                    }
                }
                break;
            }
        }
        last = c;
        c = *text++;
    }
    if (width) {
        *width = widest;
    }
    if (height) {
        *height = total;
    }
    return true;
}


// int BitFontClass::String_Pixel_Width(wchar_t const *string, int arg2) const (0x00431690)
// The width Get_Text_Area measures for the string.

int BitFontClass::String_Pixel_Width(wchar_t const *string, int arg2) const
{
    ((BitFontClass *)this)->Get_Text_Area(string, &arg2, 0, arg2);
    return arg2;
}


// 0x004316b0: function not yet identified
//
// Body: 88 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __thiscall Bold_String_Pixel_Width_004316b0(int param_1,undefined4 param_2,undefined4 param_3)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004316b0 {

struct Callee_004314b0 { unsigned int Get_Text_Area_004314b0(unsigned short *, int *, int *, int); };

struct Owner_004316b0 { unsigned int Bold_String_Pixel_Width_004316b0(unsigned int text, unsigned int width); };

unsigned int Owner_004316b0::Bold_String_Pixel_Width_004316b0(unsigned int text, unsigned int width)
{
    int self = (int)this;
    if (*(char *)(self + 0x40) != '\0') {
        *(unsigned int *)(self + 0x2c) = 2;
        ((Callee_004314b0*)(self))->Get_Text_Area_004314b0((unsigned short *)(text), (int *)(&width), (int *)(0), (int)(width));
        *(unsigned int *)(self + 0x2c) = 1;
        return width;
    }
    ((Callee_004314b0*)(self))->Get_Text_Area_004314b0((unsigned short *)(text), (int *)(&width), (int *)(0), (int)(width));
    return width;
}

}  // namespace seed_004316b0


// BitFontClass::Chars_In_Width (0x00431710): how many characters of string fit in width pixels (tabs
// to the tab width; CR/LF take no room), at most count (0: no limit); with wrap, a line break ends the
// run and a cut inside a word moves back to the word's start (unless the word fills the run).

int BitFontClass::Chars_In_Width(wchar_t const *string, int width, int count, int wrap) const
{
    int x = 0;
    int chars = 0;
    if (Data == 0 || string == 0) {
        return 0;
    }
    wchar_t character = *string;
    while (character != 0) {
        switch (character) {
        case 9:
            x += unknown_28;
            x -= x % unknown_28;
            break;
        case 10:
        case 13:
            break;
        default: {
            unsigned char *glyph = Glyph_Data(character);
            if (glyph != 0 || (glyph = (unsigned char *)unknown_08) != 0) {
                x += unknown_2c + *glyph;
            }
            break;
        }
        }
        chars++;
        if (x > width) {
            chars--;
            if (wrap != 0 && character > 0x20) {
                string--;
                int back = chars;
                while (back != 0 && *string > 0x20) {
                    back--;
                    string--;
                }
                if (back != 0) {
                    chars = back;
                }
            }
            break;
        }
        if (count != 0 && chars == count) {
            break;
        }
        if (wrap != 0 && (character == 13 || character == 10)) {
            break;
        }
        character = *++string;
    }
    return chars;
}


// 0x00431860: function not yet identified
//
// Body: 86 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: void __thiscall Bold_Text_Dimensions_00431860(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,undefined4 param_5 )
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_00431860 {

struct Callee_004314b0 { unsigned int Get_Text_Area_004314b0(unsigned short *, int *, int *, int); };

struct Owner_00431860 { void Bold_Text_Dimensions_00431860(unsigned int text, unsigned int width, unsigned int height, unsigned int limit); };

void Owner_00431860::Bold_Text_Dimensions_00431860(unsigned int text, unsigned int width, unsigned int height, unsigned int limit)
{
    int self = (int)this;
    if (*(char *)(self + 0x40) != '\0') {
        *(unsigned int *)(self + 0x2c) = 2;
        ((Callee_004314b0*)(self))->Get_Text_Area_004314b0((unsigned short *)(text), (int *)(width), (int *)(height), (int)(limit));
        *(unsigned int *)(self + 0x2c) = 1;
        return;
    }
    ((Callee_004314b0*)(self))->Get_Text_Area_004314b0((unsigned short *)(text), (int *)(width), (int *)(height), (int)(limit));
    return;
}

}  // namespace seed_00431860


// 0x004318c0: function not yet identified
//
// Body: 13 bytes (frozen inventory bound).
// Referenced from 0 sites.
// Ghidra prototype: undefined4 __thiscall Set_Tab_Width_004318c0(int param_1,undefined4 param_2)
//
// Types and names are Ghidra placeholders;
// rename and retype as the purpose becomes known.
//

namespace seed_004318c0 {

struct Owner_004318c0 { unsigned int Set_Tab_Width_004318c0(unsigned int width); };

unsigned int Owner_004318c0::Set_Tab_Width_004318c0(unsigned int width)
{
    int self = (int)this;
    unsigned int previous;

    previous = (unsigned int)(*(unsigned int *)(self + 0x28));
    *(unsigned int *)(self + 0x28) = width;
    return previous;
}

}  // namespace seed_004318c0


// void BitFontClass::Set_Tab_Origin(int value) (0x004318D0)
// Stores the value at +0x20.

void BitFontClass::Set_Tab_Origin(int value)
{
    unknown_20 = value;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// int BitFontClass::Draw_Char(wchar_t character, int x, int y, int color_arg) (0x004318E0): draws one
// character at x, y on the locked surface (a tab moves to the next tab stop from unknown_20); color -1: the
// font colour; a character without a glyph uses Missing_Glyph. Clipped to ClipBox when Clipping is set.
// The x after the character. From ra2-decomp BitFont::Blit (YR 0x00434120), adapted to our names.



int BitFontClass::Draw_Char(wchar_t character, int x, int y, int color_arg)
{
    if (character == 9) {
        x += unknown_28;
        return x - (x - unknown_20) % unknown_28;
    }
    unsigned short color = Color;
    if (color_arg != -1) color = color_arg;
    unsigned char *glyph = Glyph_Data(character);
    if (!glyph) {
        glyph = (unsigned char *)Missing_Glyph(&color);
        if (!glyph) return x;
    }
    if (!Buffer) return x;
    unsigned char *data = glyph + 1;
    int width = *glyph;
    int height = Data->unknown_08;
    if (Clipping) {
        tagRECT rect;
        tagRECT clipped;
        SetRect((RECT *)&rect, x, y, x + width - 1, y + height - 1);
        if (!Intersect_Rect(&clipped, &rect, &ClipBox)) return unknown_2c + width + x;
        if (rect.left < clipped.left || rect.right > clipped.right || rect.top < clipped.top || rect.bottom > clipped.bottom) {
            int skip = clipped.top - rect.top;
            int rows = clipped.bottom - clipped.top + 1;
            y += skip;
            data += Width * skip;
            short *line = (short *)Buffer + (Pitch * y + x);
            int clipL = clipped.left;
            int clipR = clipped.right;
            while (rows--) {
                int w = width;
                int cx = x;
                short *d = line;
                unsigned char *p = data;
                do {
                    unsigned char bits = *p++;
                    if (bits == 0) {
                        d += 8;
                        w -= 8;
                        cx += 8;
                        continue;
                    }
                    BLIT_CLIPPED(0x80) if (w <= 0) break;
                    BLIT_CLIPPED(0x40) if (w <= 0) break;
                    BLIT_CLIPPED(0x20) if (w <= 0) break;
                    BLIT_CLIPPED(0x10) if (w <= 0) break;
                    BLIT_CLIPPED(8) if (w <= 0) break;
                    BLIT_CLIPPED(4) if (w <= 0) break;
                    BLIT_CLIPPED(2) if (w <= 0) break;
                    BLIT_CLIPPED(1)
                } while (w > 0);
                data += Width;
                line += Pitch;
                y++;
            }
            return unknown_2c + width + x;
        }
    }
    short *line = (short *)Buffer + (Pitch * y + x);
    while (height--) {
        int w = width;
        unsigned char *p = data;
        short *d = line;
        do {
            unsigned char bits = *p++;
            if (bits == 0) {
                d += 8;
                w -= 8;
                continue;
            }
            BLIT_PLAIN(0x80) if (w <= 0) break;
            BLIT_PLAIN(0x40) if (w <= 0) break;
            BLIT_PLAIN(0x20) if (w <= 0) break;
            BLIT_PLAIN(0x10) if (w <= 0) break;
            BLIT_PLAIN(8) if (w <= 0) break;
            BLIT_PLAIN(4) if (w <= 0) break;
            BLIT_PLAIN(2) if (w <= 0) break;
            BLIT_PLAIN(1)
        } while (w > 0);
        data += Width;
        line += Pitch;
    }
    return unknown_2c + width + x;
}


// BitFontClass::Draw_String (0x00431CC0): draws the text from x (CR and LF skipped; at most count
// characters, 0: all); with fade, the first fade characters fade toward unknown_rgb_ab6849 and the
// text stops after them (the result is then the full text's end). The next x.

int BitFontClass::Draw_String(wchar_t const *text, int x, int y, int count, int fade)
{
    int index = 0;
    wchar_t character = *text++;
    unsigned short color = Color;
    if (character != 0) {
        int step = fade - 1;
        int ratio = (9 - fade) * 31;
        while (character != 0) {
            if (fade != 0) {
                if (index >= fade) {
                    Color = color;
                    int width;
                    Get_Text_Area(text, &width, 0, 0);
                    return width + x;
                }
                if (step < 8) {
                    RGBClass rgb = Pixel_To_RGB(color);
                    rgb.Adjust(ratio, unknown_rgb_ab6849);
                    Color = (unsigned short)(((rgb.Red >> unknown_shift_852fbc) << unknown_shift_852fb8)
                                           | ((rgb.Blue >> unknown_shift_852fc4) << unknown_shift_852fc0)
                                           | ((rgb.Green >> unknown_shift_852fcc) << unknown_shift_852fc8));
                }
                index++;
                ratio += 31;
                step--;
            }
            if (character != 13 && character != 10) {
                x = Draw_Char(character, x, y, -1);
            }
            if (count != 0 && --count == 0) {
                break;
            }
            character = *text++;
        }
    }
    Color = color;
    return x;
}


// unsigned char *BitFontClass::Glyph_Data(wchar_t character) const (0x00431E80)
// The character's glyph (its width byte, then the bitmap), or null.

unsigned char *BitFontClass::Glyph_Data(wchar_t character) const
{
    unsigned char *glyph = 0;
    if (Data) {
        unsigned index = Data->Map[character];
        if (index) {
            glyph = Data->Glyphs + Data->GlyphSize * (index - 1);
        }
    }
    return glyph;
}


// void BitFontClass::Make_Missing_Char() (0x00431EC0)
// Keeps an inverted copy of the 'X' glyph in unknown_08.

// Never expanded inline in its unit (both callers call it).
#pragma auto_inline(off)
void BitFontClass::Make_Missing_Char()
{
    unknown_08 = malloc(Data->unknown_08 * Data->Width + 2);
    unsigned char *glyph = Glyph_Data('X');
    if (glyph) {
        *(unsigned char *)unknown_08 = *glyph++;
        unsigned char *out = (unsigned char *)unknown_08 + 1;
        for (unsigned y = 0; y < (unsigned)Data->unknown_08; y++) {
            for (unsigned x = 0; x < (unsigned)Data->Width; x++) {
                *out++ = ~*glyph++;
            }
        }
    }
}
#pragma auto_inline(on)


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void *BitFontClass::Missing_Glyph(unsigned short *color) (0x00431F50): the fallback glyph (unknown_08),
// flipping the colour with 0x5555 when one is given. From ra2-decomp BitFont::FUN_00434790 (YR 0x00434790).

void *BitFontClass::Missing_Glyph(unsigned short *color)
{
    if (color) *color ^= 0x5555;
    return unknown_08;
}


// void BitFontClass::Widen_Space() (0x00431F70)
// One pixel taller, with the space one pixel wider.

void BitFontClass::Widen_Space()
{
    if (Data) {
        Height++;
        (*Glyph_Data(' '))++;
    }
}


// void BitFontClass::Set_Wide_Space() (0x00431FA0)
// One pixel taller, with the ideographic space (U+3000) twice as wide as the space.

void BitFontClass::Set_Wide_Space()
{
    if (Data) {
        Height++;
        unsigned char *space = Glyph_Data(' ');
        unsigned char *wide = Glyph_Data(0x3000);
        if (wide) {
            if (space) {
                *wide = *space << 1;
            } else {
                *wide = *(unsigned char *)Data;
            }
        }
    }
}


// bool BitFontClass::Intersect_Rect(tagRECT *result, tagRECT const *a, tagRECT const *b) (0x00432000)
// The overlap of two rectangles (edges inclusive); 0,0,-1,-1 and false
// when they do not meet.

bool BitFontClass::Intersect_Rect(tagRECT *result, tagRECT const *a, tagRECT const *b)
{
    if (a->top <= b->bottom && b->top <= a->bottom && a->left <= b->right && b->left <= a->right) {
        *result = *a;
        if (result->top < b->top) {
            result->top = b->top;
        }
        if (result->left < b->left) {
            result->left = b->left;
        }
        if (result->bottom > b->bottom) {
            result->bottom = b->bottom;
        }
        if (result->right > b->right) {
            result->right = b->right;
        }
        return true;
    }
    result->left = 0;
    result->top = 0;
    result->right = -1;
    result->bottom = -1;
    return false;
}


// void BitFontClass::Set_Surface(Surface *surface) (0x004320B0)
// Draws into the locked surface; an unset clip box becomes the whole
// surface, a set one is clipped to it.

void BitFontClass::Set_Surface(Surface *surface)
{
    Buffer = surface->Lock();
    Pitch = (unsigned)surface->Stride() >> 1;
    tagRECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = surface->Get_Width() - 1;
    rect.bottom = surface->Get_Height() - 1;
    if (!(ClipBox.right | ClipBox.bottom | ClipBox.top | ClipBox.left)) {
        ClipBox.left = rect.left;
        ClipBox.top = rect.top;
        ClipBox.right = rect.right;
        ClipBox.bottom = rect.bottom;
    } else {
        Intersect_Rect(&ClipBox, &ClipBox, &rect);
    }
}


// void BitFontClass::Release_Surface(Surface *surface) (0x00432150)
// Unlocks the surface the font drew on and forgets its buffer.

void BitFontClass::Release_Surface(Surface *surface)
{
    surface->Unlock();
    Buffer = 0;
    Pitch = 0;
}


// int BitFontClass::Char_Pixel_Width(wchar_t character) const (0x00432170)
// The glyph's width byte (0 without a glyph).

int BitFontClass::Char_Pixel_Width(wchar_t character) const
{
    unsigned char *glyph = Glyph_Data(character);
    if (glyph) {
        return *glyph;
    }
    return 0;
}


// void *BitFontClass::`scalar deleting destructor'(unsigned int flags) (0x00432200)
// Compiler-generated with the vtable (the destructor inlined). No source of its own.
