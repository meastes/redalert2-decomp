// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// From ra2-decomp (YR 0x004F4E40, src/hicolor.cpp).
// void __fastcall rgb24_to_rgb1555(unsigned char *buf, int rows, int columns, int count, int max_opacity) -- 0x004E1350
// Converts rows * columns * count four-byte pixels in place into 16-bit 1-5-5-5 values; the top bit is set
// when the first byte of the pixel is zero and max_opacity is zero. No direct callers.

#define PIXEL_RGB555(r,g,b) 		( (((r) & 0xF8) << 7) | (((g) & 0xF8) << 2) | (((b) & 0xF8) >> 3) )

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
