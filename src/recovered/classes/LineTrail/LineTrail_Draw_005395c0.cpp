// LineTrail::Draw() (0x005395C0): from the newest point on, draws a line to each next live point
// with the older point's intensity, at the depths of both points, until a faded or unused point or
// the ring wraps back to the newest.
// From ra2-decomp 0x00556C00 (YR identical twin, src/link2.cpp LineTrail::Draw):
// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// @ref ?Coord_To_Pixel@Tactical@@QAE_NABUCoordStruct@@AAUPoint2D@@@Z=006a29d0
// @ref ?Unknown_006A2970@@YIHH@Z=006a2970
// @ref ?unknown_tactical_83996c@@3PAVTactical@@A=0083996c
// @ref ?unknown_rect_839630@@3VRect@@A=00839630
// @ref ?unknown_surface_839964@@3PAVSurface@@A=00839964
// @ref ?Draw_Trail_Line_4AE9C0@DSurface@@QAE_NABVRect@@ABUPoint2D@@1ABVRGBClass@@HHH@Z=004ae9c0
#include "shared/dsurface.hpp"
#include "shared/tactical.hpp"
#include "shared/tool_tip.hpp"
#include "shared/line_trail.hpp"

void LineTrail::Draw()
{
    int slot = ActiveSlot + 1;
    if (slot >= 32) slot = 0;

    LineTrailNode *to = &Trails[ActiveSlot];
    if (to->Position == unknown_geometry_l) return;

    for (;;) {
        LineTrailNode *from = to;
        to = &Trails[slot];
        if (from->Value == 0 || to->Position == unknown_geometry_l) break;

        Point2D start;
        Point2D end;
        unknown_tactical_83996c->Coord_To_Pixel(from->Position, start);
        unknown_tactical_83996c->Coord_To_Pixel(to->Position, end);
        int start_height = -2 - Unknown_006A2970(from->Position.Z);
        int end_height = -2 - Unknown_006A2970(to->Position.Z);
        ((DSurface *)unknown_surface_839964)->Draw_Trail_Line_4AE9C0(unknown_rect_839630, start, end, Color,
                                                                     from->Value, start_height, end_height);

        slot++;
        if (slot >= 32) slot = 0;
        if (slot == ActiveSlot) break;
    }
}
