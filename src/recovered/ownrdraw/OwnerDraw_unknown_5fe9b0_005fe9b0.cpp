// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// Darkens the left_width band and the top_height band of area on a locked 16-bit surface to half (0x005FE9B0).
// Adapted from ra2-decomp src/ownrdraw.cpp FUN_006219D0 (gamemd twin 0x006219D0, matched there); see THIRD_PARTY.md.
#include "shared/surface.hpp"
#include "shared/owner_draw_dialog_control.hpp"
namespace OwnerDraw {
void __fastcall unknown_5fe9b0(Rect const &area, Surface &surface, int left_width, int top_height)
{
    unsigned short *pixels = (unsigned short *)surface.Lock();

    if (pixels != 0) {
        int stride = surface.Stride() / 2;
        int y;

        for (y = area.Y; y < area.Y + area.Height; ++y) {
            int index = y * stride + area.X;

            for (int x = area.X; x < area.X + left_width; ++x) {
                unsigned short pixel = pixels[index];

                pixels[index] = ((pixel >> 1) & ((unknown_red_green_masks_a763dc[0] >> 1) & (unsigned int)unknown_red_green_masks_a763dc[0])) |
                                ((pixel >> 1) & ((unknown_red_green_masks_a763dc[1] >> 1) & unknown_red_green_masks_a763dc[1])) |
                                ((pixel >> 1) & (unknown_blue_mask_a763e0 >> 1));

                index++;
            }
        }

        for (y = area.Y; y < area.Y + top_height; ++y) {
            int index = y * stride + area.X + left_width;

            for (int x = area.X + left_width; x < area.Width + area.X; ++x) {
                unsigned short pixel = pixels[index];

                pixels[index] = ((pixel >> 1) & ((unknown_red_green_masks_a763dc[0] >> 1) & (unsigned int)unknown_red_green_masks_a763dc[0])) |
                                ((pixel >> 1) & ((unknown_red_green_masks_a763dc[1] >> 1) & unknown_red_green_masks_a763dc[1])) |
                                ((pixel >> 1) & (unknown_blue_mask_a763e0 >> 1));

                index++;
            }
        }

        surface.Unlock();
    }
}
}
