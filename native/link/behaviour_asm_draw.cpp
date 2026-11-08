// Behaviour rewrites of the drawing helpers written in assembly (winasm.asm):
// the spot-light brighteners and the 2x interpolating copies. For the native
// tree only. NOT EXACT and not original code: portable C++ written from the
// adopted TibSun archive procedures in src/recovered/asm/winasm.asm (each one
// proven byte-identical to the pinned target in
// manifests/original_target/assembly.json; TibSun archive text, GPL-3.0-or-later
// with EA's section 7 terms, THIRD_PARTY.md). The per-format constants below
// are the BRIGHTEN_COLOR / MMX_BRIGHTEN_COLOR macro constants of that file.
// Tests: native/tests/asm_check.cpp (native-asm-check). Manifest:
// native/behaviour_rewrites.json.
#include <stdint.h>
#include <string.h>

// ---------------------------------------------------------------------------
// Brighten_Color_565 0x0079813A, _655 0x00798217, _556 0x007982F4, _555
// 0x007983D1. For every pixel whose multiplier byte s is nonzero, each colour
// component c (expanded to 8 bits as the format's top bits) becomes
// min(255, c + (c * s >> 8)), then is packed back. Rows: the multiplier buffer
// advances by mulbuff_width bytes, the colour buffer by color_buff_width
// bytes (both pitches). Width and height count down to zero (callers pass at
// least 1; zero returns here, the original would wrap).
// ---------------------------------------------------------------------------
namespace {
struct BrightenFormat {
    // CONSTANT_0..12 of BRIGHTEN_COLOR (CONSTANT_4/5/11 are 8 in every format).
    int c0, c1, c2, c3, c6, c7, c8, c9, c10, c12;
};
BrightenFormat const Brighten565 = {8, 3, 0xF8, 0xFC, 3, 2, 11, 5, 3, 3};
BrightenFormat const Brighten655 = {8, 2, 0xFC, 0xF8, 2, 3, 10, 5, 3, 3};
BrightenFormat const Brighten556 = {8, 3, 0xF8, 0xF8, 3, 3, 11, 6, 2, 2};
BrightenFormat const Brighten555 = {7, 2, 0xF8, 0xF8, 3, 3, 10, 5, 3, 3};

inline uint32_t brighten_component(uint32_t c, uint32_t s)
{
    uint32_t v = c + ((c * s) >> 8);   // add bl,dl / setnb / dec / or: saturate the byte
    return v > 255 ? 255 : v;
}

void brighten(BrightenFormat const &f, void *sptr, void *dptr, int spitch, int dpitch, int width, int height)
{
    if (width == 0 || height == 0) return;
    unsigned char *srow = (unsigned char *)sptr;
    unsigned char *drow = (unsigned char *)dptr;
    for (uint32_t y = (uint32_t)height; y != 0; --y, srow += spitch, drow += dpitch) {
        for (uint32_t x = 0; x != (uint32_t)width; ++x) {
            uint32_t s = srow[x];
            if (s == 0) continue;
            uint16_t pixel;
            memcpy(&pixel, drow + 2 * x, 2);
            uint32_t p = pixel;
            uint32_t r = brighten_component((p >> f.c0) & (uint32_t)f.c2, s);
            uint32_t g = brighten_component((p >> f.c1) & (uint32_t)f.c3, s);
            uint32_t b = brighten_component((p << f.c10) & 0xFF, s);
            uint16_t out = (uint16_t)(((r >> f.c6) << f.c8) | ((g >> f.c7) << f.c9) | (b >> f.c12));
            memcpy(drow + 2 * x, &out, 2);
        }
    }
}

// ---------------------------------------------------------------------------
// MMX_Brighten_Color_565 0x007984C0, _555 0x00798602, _556 0x00798740, _655
// 0x00798880. The same brightening through a caller-built table of 65,536
// dwords (SpotLight's: R << 16 | G << 8 | B per 16-bit pixel): punpcklbw +
// psrlw 8 widens the entry's bytes B, G, R, 0 to words, pmullw/psrlw 8 scales
// by s (the fourth word by 0), paddusb adds the original byte with unsigned
// saturation, psrlw shifts each word by CONSTANT_2; then B >> CONSTANT_3,
// G << CONSTANT_4, R << CONSTANT_5 are combined. Formats other than 655 mask
// R with CONSTANT_6 << 8; 655 instead masks G with 0x423A0A60 (the macro's
// constant, kept as is; the archive procedure matches the target).
// ---------------------------------------------------------------------------
struct MMXFormat {
    int c2, c3, c4, c5;
    uint32_t c6;
};
MMXFormat const MMX565 = {2, 1, 5, 10, 0xF8};
MMXFormat const MMX555 = {3, 0, 5, 10, 0x7C};
MMXFormat const MMX556 = {2, 0, 6, 10, 0xF8};
MMXFormat const MMX655 = {2, 1, 4, 10, 0x423A0A60};

void mmx_brighten(MMXFormat const &f, void *sptr, void *dptr, int spitch, int dpitch, int width, int height,
                  void *table)
{
    if (width == 0 || height == 0) return;
    unsigned char const *entries = (unsigned char const *)table;
    unsigned char *srow = (unsigned char *)sptr;
    unsigned char *drow = (unsigned char *)dptr;
    for (uint32_t y = (uint32_t)height; y != 0; --y, srow += spitch, drow += dpitch) {
        for (uint32_t x = 0; x != (uint32_t)width; ++x) {
            uint32_t s = srow[x];
            if (s == 0) continue;
            uint16_t pixel;
            memcpy(&pixel, drow + 2 * x, 2);
            unsigned char const *e = entries + 4 * (uint32_t)pixel;
            uint32_t w[3];
            for (int k = 0; k < 3; ++k) {
                uint32_t c = e[k];
                uint32_t t = ((c * s) & 0xFFFF) >> 8;
                uint32_t v = t + c;            // paddusb on the low byte (high bytes are 0)
                w[k] = (v > 255 ? 255 : v) >> f.c2;
            }
            uint32_t ebx = (w[0] & 0xFF) >> f.c3;
            uint32_t eax = (w[1] & 0xFF) << f.c4;
            uint32_t ecx = (w[2] & 0xFF) << f.c5;
            if (f.c6 == 0x423A0A60) {
                ebx |= eax & f.c6;
            } else {
                ebx |= eax;
                ecx &= f.c6 << 8;
            }
            ebx |= ecx;
            uint16_t out = (uint16_t)ebx;
            memcpy(drow + 2 * x, &out, 2);
        }
    }
}
}

extern "C" {
void Brighten_Color_555(void *sptr, void *dptr, int spitch, int dpitch, int width, int height)
{ brighten(Brighten555, sptr, dptr, spitch, dpitch, width, height); }
void Brighten_Color_556(void *sptr, void *dptr, int spitch, int dpitch, int width, int height)
{ brighten(Brighten556, sptr, dptr, spitch, dpitch, width, height); }
void Brighten_Color_565(void *sptr, void *dptr, int spitch, int dpitch, int width, int height)
{ brighten(Brighten565, sptr, dptr, spitch, dpitch, width, height); }
void Brighten_Color_655(void *sptr, void *dptr, int spitch, int dpitch, int width, int height)
{ brighten(Brighten655, sptr, dptr, spitch, dpitch, width, height); }
void MMX_Brighten_Color_555(void *sptr, void *dptr, int spitch, int dpitch, int width, int height, void *buffer)
{ mmx_brighten(MMX555, sptr, dptr, spitch, dpitch, width, height, buffer); }
void MMX_Brighten_Color_556(void *sptr, void *dptr, int spitch, int dpitch, int width, int height, void *buffer)
{ mmx_brighten(MMX556, sptr, dptr, spitch, dpitch, width, height, buffer); }
void MMX_Brighten_Color_565(void *sptr, void *dptr, int spitch, int dpitch, int width, int height, void *buffer)
{ mmx_brighten(MMX565, sptr, dptr, spitch, dpitch, width, height, buffer); }
void MMX_Brighten_Color_655(void *sptr, void *dptr, int spitch, int dpitch, int width, int height, void *buffer)
{ mmx_brighten(MMX655, sptr, dptr, spitch, dpitch, width, height, buffer); }
}

// ---------------------------------------------------------------------------
// Asm_Interpolate 0x00799241, Asm_Interpolate_Line_Double 0x007992E6,
// Asm_Interpolate_Line_Interpolate 0x0079943B (with Interpolate_Single_Line
// 0x007993B0 and Interpolate_Between_Lines 0x00799412). T(a, b) is
// PaletteInterpolationTable[b][a] (byte a + 256 * b of the table, the blend
// of colours a and b).
// One source line of w pixels (w even) becomes 2w pixels: for each pair
// p0 p1 (and the following p2): p0, T(p0,p1), p1, T(p1,p2); the last pair
// gives p0, T(p0,p1), p1, 0. Source lines are consecutive (w bytes apart);
// destination lines are dest_width bytes apart (the caller passes twice the
// pitch, so every other line is left as is by Asm_Interpolate).
// Line_Double also writes each line at dest_width/2 below.
// Line_Interpolate writes line 0, then for each next source line the blend
// T(previous, next) pixel by pixel at dest_width/2 below, and so on; the
// final destination line is the interpolation of the source line AFTER the
// last one (the original reads that line; reproduced as is).
// The original keeps its line buffers in .data (640 dwords each: 1,280 source
// pixels); so do these. Counts of zero (lines, (w-2)/2) do not loop here.
// ---------------------------------------------------------------------------
extern "C" unsigned char PaletteInterpolationTable[256][256];

namespace {
uint32_t TopLine[640];
uint32_t BottomLine[640];
uint32_t LineBuffer[640];

inline unsigned char blend(unsigned a, unsigned b) { return (&PaletteInterpolationTable[0][0])[a + 256 * b]; }

// One line; returns the source pointer after it (the asm advances esi by 2 per pair).
unsigned char const *interpolate_line(unsigned char const *s, unsigned char *d, int source_width)
{
    uint32_t pairs = (uint32_t)(source_width - 2) >> 1;
    for (uint32_t i = 0; i < pairs; ++i, s += 2, d += 4) {
        d[0] = s[0];
        d[1] = blend(s[0], s[1]);
        d[2] = s[1];
        d[3] = blend(s[1], s[2]);
    }
    d[0] = s[0];
    d[1] = blend(s[0], s[1]);
    d[2] = s[1];
    d[3] = 0;
    return s + 2;
}
}

extern "C" void Asm_Interpolate(unsigned char *src_ptr, unsigned char *dest_ptr, int lines, int src_width,
                                int dest_width)
{
    unsigned char const *s = src_ptr;
    unsigned char *d = dest_ptr;
    for (uint32_t n = (uint32_t)lines; n != 0; --n, d += dest_width) s = interpolate_line(s, d, src_width);
}

extern "C" void Asm_Interpolate_Line_Double(unsigned char *src_ptr, unsigned char *dest_ptr, int lines,
                                            int src_width, int dest_width)
{
    unsigned char const *s = src_ptr;
    unsigned char *d = dest_ptr;
    unsigned char *line = (unsigned char *)LineBuffer;
    uint32_t written = (((uint32_t)(src_width - 2) >> 1) + 1) * 4;
    uint32_t half = (uint32_t)dest_width >> 1;
    for (uint32_t n = (uint32_t)lines; n != 0; --n) {
        s = interpolate_line(s, line, src_width);   // the asm stores each dword to both
        memcpy(d, line, written);
        d += half;
        memcpy(d, line, ((uint32_t)src_width >> 1) * 4);
        d += half;
    }
}

extern "C" void Asm_Interpolate_Line_Interpolate(unsigned char *src_ptr, unsigned char *dest_ptr, int lines,
                                                 int src_width, int dest_width)
{
    uint32_t bytes = ((uint32_t)src_width >> 1) * 4;
    uint32_t pitch = (uint32_t)dest_width >> 1;
    unsigned char *next = (unsigned char *)TopLine;
    unsigned char *last = (unsigned char *)BottomLine;
    unsigned char *s = src_ptr;
    unsigned char *d = dest_ptr;
    unsigned char *swap;
    interpolate_line(s, next, src_width);
    swap = next; next = last; last = swap;
    s += src_width;
    for (uint32_t n = (uint32_t)lines - 1; n != 0 && n != 0xFFFFFFFFu; --n) {
        interpolate_line(s, next, src_width);
        unsigned char *between = (unsigned char *)LineBuffer;
        for (uint32_t i = 0; i < 2 * (uint32_t)src_width; ++i) between[i] = blend(last[i], next[i]);
        memcpy(d, last, bytes);
        d += pitch;
        memcpy(d, between, bytes);
        d += pitch;
        s += src_width;
        swap = next; next = last; last = swap;
    }
    interpolate_line(s, next, src_width);
    memcpy(d, next, bytes);
}
