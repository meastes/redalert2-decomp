/*******************************************************************************
 *                                O P E N  T S
 *******************************************************************************
 * SPDX-License-Identifier: GPL-3.0-or-later
 * Copyright 2025 Electronic Arts Inc.
 * Copyright 2026 OpenTS contributors
 *
 * Contains material derived from Electronic Arts source code.
 * Modified by OpenTS contributors, 2026.
 * EA's GPLv3 Section 7 additional terms and supplemental warranty
 * disclaimers apply; see LICENSE.md.
 ******************************************************************************/
// bool XSurface::Fill_Rect(Rect const &cliprect, Rect const &fillrect, int color) (0x007743C0, slot 4)
// From mischa85/RedAlert2 commit 5a79accc src/xsurface.cpp (TibSun code/xsurface.cpp:848), adapted
// to our shared headers. surface_quick_fill's `_asm` block is the TibSun archive's own
// (TibSun xsurface.cpp:806, instructions unchanged); VC6 expands it inline in both 16-bit cases.
#include "shared/surface.hpp"
#include "shared/crt_string.hpp"
// @ref ?Intersect@@YI?AVRect@@ABV1@0PAH1@Z=0041fdf0
// @ref ??0Rect@@QAE@ABURectPoint2D@@HH@Z=00453e40
// @ref ?Top_Left@Rect@@QBE?AURectPoint2D@@XZ=00435560

// inline asm functions don't have a return statement
#pragma warning(disable: 4035)

// Fills count longwords with color and returns the pointer just past the last one written,
// so the caller can finish an odd trailing pixel (TibSun xsurface.cpp:793).
static void *surface_quick_fill(void *buf, int count, int color)
{
    _asm {
        push edi

        mov ecx, [count]
        mov edi, [buf]

        cmp ecx, 0
        jle short $end

        mov eax, [color]
        rep stosd

    $end:
        mov eax, edi
        pop edi
    }
    // return is in eax
}
#pragma warning(default: 4035)

bool XSurface::Fill_Rect(Rect const &cliprect, Rect const &fillrect, int color)
{
    if (!fillrect.Is_Valid()) return false;

    // Ensure that the requested clipping rectangle is actually legal.
    Rect wrect = cliprect.Intersect(Get_Rect());

    // Clip the rectangle to be filled against the clipping rectangle supplied.
    Rect crect = wrect.Intersect(fillrect.Bias_To(cliprect));
    if (!crect.Is_Valid()) return false;

    int height = crect.Height;
    int width = crect.Width;
    int pitch = Stride();

    void *buffer = Lock(crect.Top_Left());
    if (buffer != 0) {
        if (Bytes_Per_Pixel() == 1) {
            for (int y = 0; y < crect.Height; y++) {
                memset(buffer, color, crect.Width);
                buffer = ((unsigned char *)buffer) + pitch;
            }
        } else {
            switch ((unsigned int)buffer & 3) {
                case 0: {
                    int odd_pixel = width & 1;
                    width >>= 1;
                    int doubled_color = (color << 16) | color;
                    for (int i = 0; i < height; i++) {
                        void *tail = surface_quick_fill(buffer, width, doubled_color);
                        if (odd_pixel) {
                            *(unsigned short *)tail = color;
                        }
                        buffer = (unsigned char *)buffer + pitch;
                    }
                    break;
                }

                case 2: {
                    if (width <= 1) {
                        if (width == 1) {
                            for (int i = 0; i < height; i++) {
                                *(unsigned short *)buffer = color;
                                buffer = (unsigned char *)buffer + pitch;
                            }
                        }
                    } else {
                        width--;
                        int odd_pixel = width & 1;
                        width >>= 1;
                        int doubled_color = (color << 16) | color;
                        for (int i = 0; i < height; i++) {
                            *(unsigned short *)buffer = color;
                            void *tail = surface_quick_fill((unsigned short *)buffer + 1, width, doubled_color);
                            if (odd_pixel) {
                                *(unsigned short *)tail = color;
                            }
                            buffer = (unsigned char *)buffer + pitch;
                        }
                    }
                    break;
                }

                default: {
                    for (int y = 0; y < height; y++) {
                        for (int x = 0; x < width; x++) {
                            ((unsigned short *)buffer)[x] = (unsigned short)color;
                        }
                        buffer = ((unsigned char *)buffer) + pitch;
                    }
                    break;
                }
            }
        }
        Unlock();
        return true;
    }
    return false;
}
