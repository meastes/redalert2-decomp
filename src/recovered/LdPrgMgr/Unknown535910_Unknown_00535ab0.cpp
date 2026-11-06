// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// Point2D Unknown535910::Unknown_00535AB0() -- 0x00535AB0
// The loading screen's text position: in a campaign inside the text bar,
// otherwise offset from the picture's corner, both by the screen size.
// from ra2-decomp 0x00552BE0 (LoadProgressManager::Text_Origin, src/ldprgmgr.cpp)
#include "shared/unknown_globals.hpp"
#include "shared/rect.hpp"
// @ref ?unknown_screen_widths_7add68@@3PAHA=007add68
// @ref ?Unknown_Point_Copy_006F1AE0@@YI?AUPoint2D@@PBU1@@Z=006f1ae0

Point2D __fastcall Unknown_Point_Copy_006F1AE0(Point2D const *source);

Point2D Unknown535910::Unknown_00535AB0()
{
    Point2D p;
    if (unknown_game_mode_a3d298 == 0) {
        p.X = unknown_28.X;
        if (unknown_8523e0 == unknown_screen_widths_7add68[0]) {
            p.X += 0x54;
        } else {
            p.X += 0xa4;
        }
        p.Y = unknown_28.Y + 7;
    } else {
        p = Unknown_Point_Copy_006F1AE0((Point2D const *)&unknown_18);
        if (unknown_8523e0 == unknown_screen_widths_7add68[0]) {
            p.X += 0xc;
            p.Y += 0x100;
        } else {
            p.X += 0x10;
            p.Y += 0x141;
        }
    }
    return p;
}
