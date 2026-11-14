// hicolor.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/hicolor.json carries the addresses and the rows that
// stay in their own files.


#define PIXEL_RGB555(r,g,b) 		( (((r) & 0xF8) << 7) | (((g) & 0xF8) << 2) | (((b) & 0xF8) >> 3) )


// void __fastcall rgb32_to_rgb24_clamped(unsigned char *palette, int count) -- 0x004E12D0
// Clears the three low bits of the three colour components of `count` four-byte pixels.
void __fastcall rgb32_to_rgb24_clamped_004e12d0(unsigned char *palette, int count)
{
    for (int index = count; index > 0; index--) {
        *palette++ &= 0xf8;
        *palette++ &= 0xf8;
        *palette++ &= 0xf8;
        palette++;
    }
}


// void __fastcall rgb24_to_rgb555(unsigned char *pixels, unsigned int width, unsigned int height, unsigned int depth) -- 0x004E1300
// Packs width * height * depth three-byte colours (red, green, blue) in place into 16-bit 5-5-5 values
// (the three low bits of red and green are dropped, blue is shifted down). No direct callers; the three
// factors of the count and the owner are not identified.
void __fastcall rgb24_to_rgb555_004e1300(unsigned char *pixels, unsigned int width, unsigned int height, unsigned int depth)
{
    unsigned short *out = (unsigned short *)pixels;
    unsigned char *in = pixels;
    for (unsigned int count = width * height * depth; count > 0; count--) {
        unsigned short red = in[0] & 0xf8;
        unsigned short green = in[1] & 0xf8;
        unsigned short blue = in[2] >> 3;
        *out++ = (unsigned short)(((red << 5 | green) << 2) | blue);
        in += 3;
    }
}


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// From ra2-decomp (YR 0x004F4E40, src/hicolor.cpp).
// void __fastcall rgb24_to_rgb1555(unsigned char *buf, int rows, int columns, int count, int max_opacity) -- 0x004E1350
// Converts rows * columns * count four-byte pixels in place into 16-bit 1-5-5-5 values; the top bit is set
// when the first byte of the pixel is zero and max_opacity is zero. No direct callers.


void __fastcall rgb24_to_rgb1555_004e1350(unsigned char *buf, int rows, int columns, int count, int max_opacity)
{
	unsigned len = rows * columns * count;
	unsigned char *src = buf;
	unsigned short *dst = (unsigned short *)src;
	for (unsigned i = 0; i < len; i++) {
		unsigned short accum = 0;
		if (*src == 0 && max_opacity == 0) {
			accum = 0x8000;
		}
		src++;
		accum |= PIXEL_RGB555(src[0], src[1], src[2]);
		src += 3;
		*dst = accum;
		dst++;
	}
}


// void __fastcall rgb32_to_rgb24(unsigned char *pixels, unsigned int width, unsigned int height, unsigned int depth) -- 0x004E13C0
// Packs width * height * depth four-byte pixels in place into three-byte colours (the first byte of every
// pixel is dropped). No direct callers; the three factors of the count and the owner are not identified.
void __fastcall rgb32_to_rgb24_004e13c0(unsigned char *pixels, unsigned int width, unsigned int height, unsigned int depth)
{
    unsigned char *out = pixels;
    unsigned char *in = pixels;
    for (unsigned int count = width * height * depth; count > 0; count--) {
        in += 1;
        *out = *in;
        in++;
        out++;
        *out = *in;
        in++;
        out++;
        *out = *in;
        in++;
        out++;
    }
}
