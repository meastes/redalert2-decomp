// bittext.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/bittext.json carries the addresses and the rows that
// stay in their own files.


#define RGB_DECLARE_ACCESSORS
#include "shared/text_print.hpp"
#include "shared/hicolor_pixel.hpp"
#include "shared/rgb_globals.hpp"
#include "shared/hicolor.hpp"


class Leaf_432270 { public: void get(); };
void Leaf_432270::get() { unsigned int unused_this_address = (unsigned int)this; unsigned int zero_value; unsigned int unused_value; zero_value = 0; *(unsigned int*)(0x84e9d8) = zero_value; *(unsigned int*)(0x84e9dc) = zero_value; *(unsigned int*)(0x84e9e0) = zero_value; *(unsigned int*)(0x84e9e4) = zero_value; }


// RGB accessors for Print_Box 0x00432490.
// BitTextClass::BitTextClass() (0x00432290)
// Loads the game font.

BitTextClass::BitTextClass()
{
    BitFont = new BitFontClass("GAME.FNT");
}


// BitTextClass::~BitTextClass() (0x004322D0)
// Releases the game font.

BitTextClass::~BitTextClass()
{
    BitFontClass *font = BitFont;
    delete font;
    if (font == BitFont) {
        BitFont = 0;
    }
}


// int BitTextClass::Print_Char(BitFontClass *font, Surface *surface, wchar_t character, int x, int y) (0x00432300)
// Draws one character into the surface; returns the x after it.

int BitTextClass::Print_Char(BitFontClass *font, Surface *surface, wchar_t character, int x, int y)
{
    int result = x;
    if (font) {
        font->Set_Surface(surface);
        font->Set_Tab_Origin(x);
        result = font->Draw_Char(character, x, y, -1);
        font->Release_Surface(surface);
    }
    return result;
}


// int BitTextClass::Print(BitFontClass *font, Surface *surface, wchar_t const *text, int x, int y, int arg6, int arg7) (0x00432350)
// Draws a string into the surface; returns what Draw_String does (x without a font).

int BitTextClass::Print(BitFontClass *font, Surface *surface, wchar_t const *text, int x, int y, int arg6, int arg7)
{
    int result = x;
    if (font) {
        font->Set_Surface(surface);
        font->Set_Tab_Origin(x);
        result = font->Draw_String(text, x, y, arg6, arg7);
        font->Release_Surface(surface);
    }
    return result;
}


// int BitTextClass::Print_Bold(BitFontClass *font, Surface *surface, wchar_t const *text, int x, int y, int count) (0x004323B0)
// Draws the string; with the font's unknown_40 set it is drawn twice, one
// pixel apart, with one more pixel between characters.

int BitTextClass::Print_Bold(BitFontClass *font, Surface *surface, wchar_t const *text, int x, int y, int count)
{
    int result = 0;
    if (font) {
        if (font->unknown_40) {
            font->unknown_2c = 2;
            font->Set_Surface(surface);
            font->Set_Tab_Origin(x);
            font->Draw_String(text, x, y, count, 0);
            font->Release_Surface(surface);
            int next = x + 1;
            font->Set_Surface(surface);
            font->Set_Tab_Origin(next);
            result = font->Draw_String(text, next, y, count, 0);
            font->Release_Surface(surface);
            font->unknown_2c = 1;
        } else {
            font->Set_Surface(surface);
            font->Set_Tab_Origin(x);
            result = font->Draw_String(text, x, y, count, 0);
            font->Release_Surface(surface);
        }
    }
    return result;
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// bool BitTextClass::Print_Box(...) (0x00432490): word-wrapped text in a box of width x height (0: unbounded);
// flags 1 centres, 2 right-aligns each line; with limit, at most limit characters are drawn and the last fade
// of them fade toward unknown_rgb_ab6849. False without a font or text.
// From ra2-decomp BitText::DrawText (YR 0x00434CD0), adapted to our names.

// A hicolor pixel as its three channels.
inline RGBClass Int_To_RGB(int color)
{
    return RGBClass(
        (unsigned char)((unsigned short)color >> unknown_shift_852fb8) << unknown_shift_852fbc,
        (unsigned char)((unsigned short)color >> unknown_shift_852fc8) << unknown_shift_852fcc,
        (unsigned char)((unsigned short)color >> unknown_shift_852fc0) << unknown_shift_852fc4);
}

// Three channels as a hicolor pixel.
inline int RGB_To_Int(RGBClass const &rgb)
{
    return RGB_To_Pixel(rgb.Get_Red(), rgb.Get_Green(), rgb.Get_Blue());
}

// The offset of a line linewidth wide in width: flags 1 centres it, 2 right-aligns it.
inline int Align_Offset(int width, int linewidth, int flags)
{
    int offset = 0;
    if (width > linewidth) {
        if (flags & 1) {
            offset = (width - linewidth) / 2;
        } else if (flags & 2) {
            offset = width - linewidth;
        }
    }
    return offset;
}

// The glyph of a character, or of '?' when the font has none.
inline unsigned char *Char_Bitmap(BitFontClass *font, wchar_t ch)
{
    unsigned char *glyph = font->Glyph_Data(ch);
    if (!glyph) {
        glyph = font->Glyph_Data('?');
    }
    return glyph;
}

// Draws start..end as one line; false when the character limit is reached.
inline bool Draw_Line(BitFontClass *font, wchar_t const *&start, wchar_t const *end, int x, int y, int width, int linewidth, int flags, int &drawn, int &px, int color, int limit, int fade)
{
    px = x + Align_Offset(width, linewidth, flags);
    while (start < end) {
        drawn++;
        int c = color;
        if (limit) {
            if (drawn >= limit) {
                c = -1;
            } else {
                int left = limit - drawn - 1;
                if (left < fade) {
                    RGBClass rgb = Int_To_RGB(color);
                    rgb.Adjust((255 / fade) * (fade - left), unknown_rgb_ab6849);
                    c = RGB_To_Int(rgb);
                }
            }
        }
        if (c == -1) {
            return false;
        }
        px = font->Draw_Char(*start++, px, y, c);
    }
    return true;
}

bool BitTextClass::Print_Box(BitFontClass *font, Surface *surface, wchar_t const *text, int x, int y, int width,
                             int height, int flags, int limit, int fade)
{
    int linewidth = 0;
    if (!font || !text) {
        return false;
    }

    font->Set_Surface(surface);
    font->Set_Tab_Origin(x);

    wchar_t const *start = text;
    int liney = 0;
    int spacewidth = 0;
    wchar_t const *space = 0;
    wchar_t prev = 0;
    int chars = 0;
    int color = font->Color;
    int drawn = 0;
    int px;
    wchar_t ch = *text;

    while (ch) {
        switch (ch) {
        case '\r':
        case '\n':
            if (prev != '\r') {
                if (!Draw_Line(font, start, text, x, y, width, linewidth, flags, drawn, px, color, limit, fade)) {
                    font->Release_Surface(surface);
                    return true;
                }
                linewidth = 0;
                chars = 0;
                liney += font->Height;
                if (height && liney >= height) {
                    font->Release_Surface(surface);
                    return true;
                }
                y += font->Height;
            }
            start++;
            break;

        case '\t':
            linewidth += font->unknown_28;
            linewidth -= linewidth % font->unknown_28;
            break;

        case ' ':
            spacewidth = linewidth;
            space = text;

        default:
            {
                unsigned char *glyph = Char_Bitmap(font, ch);
                if (!glyph) {
                    break;
                }
                chars++;
                int w = *glyph + font->unknown_2c;
                linewidth += w;
                if (width && linewidth > width) {
                    if (space) {
                        if (chars > 1) {
                            linewidth = spacewidth;
                            text = space;
                        }
                    } else if (chars > 1) {
                        linewidth -= w;
                        text--;
                    }
                    px = x + Align_Offset(width, linewidth, flags);
                    while (start < text) {
                        drawn++;
                        int c = color;
                        if (limit) {
                            if (drawn >= limit) {
                                c = -1;
                            } else {
                                int left = limit - drawn - 1;
                                if (left < fade) {
                                    RGBClass rgb = Int_To_RGB(color);
                                    rgb.Adjust((255 / fade) * (fade - left), unknown_rgb_ab6849);
                                    c = RGB_To_Int(rgb);
                                }
                            }
                        }
                        if (c == -1) {
                            font->Release_Surface(surface);
                            return true;
                        }
                        px = font->Draw_Char(*start++, px, y, c);
                    }
                    if (text == space) {
                        space = 0;
                        start++;
                    }
                    linewidth = 0;
                    chars = 0;
                    liney += font->Height;
                    if (height && liney >= height) {
                        font->Release_Surface(surface);
                        return true;
                    }
                    y += font->Height;
                }
            }
            break;
        }
        prev = ch;
        text++;
        ch = *text;
    }

    px = x + Align_Offset(width, linewidth, flags);
    while (start < text) {
        drawn++;
        int c = color;
        if (limit) {
            if (drawn >= limit) {
                c = -1;
            } else {
                int left = limit - drawn - 1;
                if (left < fade) {
                    RGBClass rgb = Int_To_RGB(color);
                    rgb.Adjust((255 / fade) * (fade - left), unknown_rgb_ab6849);
                    c = RGB_To_Int(rgb);
                }
            }
        }
        if (c == -1) {
            font->Release_Surface(surface);
            return true;
        }
        px = font->Draw_Char(*start++, px, y, c);
    }
    font->Release_Surface(surface);
    return true;
}


// bool BitTextClass::Print_Box_Shadow(...) (0x00432B00): Print_Box in the font's unknown_26 colour one pixel
// down and right, then in its own colour. Names are hypotheses.

bool BitTextClass::Print_Box_Shadow(BitFontClass *font, Surface *surface, wchar_t const *text, int x, int y,
                                    int width, int height, int arg8, int arg9, int arg10)
{
    if (font == 0) {
        return false;
    }
    font->Set_Tab_Origin(x);
    unsigned short shadow = font->unknown_26;
    unsigned short color = font->Set_Color(shadow);
    Print_Box(font, surface, text, x + 1, y + 1, width, height, arg8, arg9, arg10);
    font->Set_Color(color);
    return Print_Box(font, surface, text, x, y, width, height, arg8, arg9, arg10);
}


// bool BitTextClass::Print_Box_Bold(...) (0x00432BB0): Print_Box; with the font's unknown_40 set it is drawn
// twice, one pixel apart, with one more pixel between characters (as Print_Bold). Names are hypotheses.

bool BitTextClass::Print_Box_Bold(BitFontClass *font, Surface *surface, wchar_t const *text, int x, int y,
                                  int width, int height, int arg8, int arg9, int arg10)
{
    if (font == 0) {
        return false;
    }
    font->Set_Tab_Origin(x);
    bool result;
    if (font->unknown_40) {
        font->unknown_2c = 2;
        Print_Box(font, surface, text, x, y, width, height, arg8, arg9, arg10);
        result = Print_Box(font, surface, text, x + 1, y, width, height, arg8, arg9, arg10);
        font->unknown_2c = 1;
    } else {
        result = Print_Box(font, surface, text, x, y, width, height, arg8, arg9, arg10);
    }
    return result;
}


// void BitTextClass::Load_Game_Font() (0x00432C90): loads GAME.FNT into BitFont, as the constructor. No callers.

void BitTextClass::Load_Game_Font()
{
    BitFont = new BitFontClass("GAME.FNT");
}


// void BitTextClass::Free_Game_Font() (0x00432CC0): releases the game font, as the destructor. No callers.

void BitTextClass::Free_Game_Font()
{
    BitFontClass *font = BitFont;
    delete font;
    if (font == BitFont) {
        BitFont = 0;
    }
}


// void BitTextClass::Free_Font(BitFontClass *font) (0x00432CF0): deletes a font; BitFont is cleared when it was
// the game font. No callers.

void BitTextClass::Free_Font(BitFontClass *font)
{
    delete font;
    if (font == BitFont) {
        BitFont = 0;
    }
}


// void *BitTextClass::`scalar deleting destructor'(unsigned int flags) (0x00432D20)
// Compiler-generated with the vtable (the destructor inlined). No source of its own.


// The out-of-line RGB screen-pixel packing body after BitText.cpp, 00432D90.
int __fastcall RGB_To_Pixel_00432d90(int red, int green, int blue)
{
    return ((red >> unknown_shift_852fbc) << unknown_shift_852fb8)
         | ((green >> unknown_shift_852fcc) << unknown_shift_852fc8)
         | ((blue >> unknown_shift_852fc4) << unknown_shift_852fc0);
}
