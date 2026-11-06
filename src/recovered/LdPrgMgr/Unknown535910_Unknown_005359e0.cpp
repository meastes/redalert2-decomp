// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void Unknown535910::Unknown_005359E0() -- 0x005359E0
// Places the loading screen's three areas by the screen size: in a campaign
// the title bar (+0x08), picture (+0x18) and text bar (+0x28) stacked and
// centred; otherwise only the picture, centred.
// from ra2-decomp 0x00552B10 (LoadProgressManager::Compute_Rects, src/ldprgmgr.cpp)
#include "shared/unknown_globals.hpp"
// @ref ?unknown_screen_widths_7add68@@3PAHA=007add68
// @ref ?unknown_screen_heights_7add74@@3PAHA=007add74

void Unknown535910::Unknown_005359E0()
{
    int render_width = unknown_8523e0;
    bool low_res = render_width == 640;
    int width = low_res ? unknown_screen_widths_7add68[0] : unknown_screen_widths_7add68[1];
    if (unknown_game_mode_a3d298 == 0) {
        int height = (low_res ? unknown_screen_heights_7add74[0] : unknown_screen_heights_7add74[1]) - 80;
        int x = (render_width - width) / 2;
        int y = (unknown_8523e4 - height - 80) / 2;
        unknown_28.Width = unknown_18.Width = unknown_08.Width = width;
        unknown_18.X = unknown_08.X = x;
        unknown_08.Y = y;
        unknown_08.Height = 40;
        y += 40;
        unknown_18.Y = y;
        unknown_28.X = x;
        unknown_18.Height = height;
        y += height;
        unknown_28.Y = y;
        unknown_28.Height = 40;
    } else {
        int height = low_res ? unknown_screen_heights_7add74[0] : unknown_screen_heights_7add74[1];
        unknown_18.Set((render_width - width) / 2, (unknown_8523e4 - height) / 2, width, height);
    }
}
