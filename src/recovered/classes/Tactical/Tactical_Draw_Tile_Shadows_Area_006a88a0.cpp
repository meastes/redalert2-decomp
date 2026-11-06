// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void Tactical::Draw_Tile_Shadows(int, Rect const &area) -- 0x006A88A0
// From ra2-decomp Tactical::Draw_Tile_Shadows(int, Rect const &) (YR 0x006D8260).
// Purpose: draws the shadow casts of the cells inside area, clipped to it;
// the scan starts at the area's map corner moved back onto the map.
#include "shared/tactical.hpp"
#include "shared/display.hpp"
#include "shared/map_view.hpp"
#include "shared/cell.hpp"

// TibSun inline.h Coord_Whole: the coordinate without its sub-cell part.
static inline CoordStruct Coord_Whole(CoordStruct const &coord)
{
    int x = coord.X / 256;
    int y = coord.Y / 256;

    return CoordStruct(x * 256, y * 256, coord.Z);
}

static inline void Set_Ground_Position(CoordStruct &coord, Point2D const &point)
{
    coord.X = point.X;
    coord.Y = point.Y;
    coord.Z = 0;
}

void Tactical::Draw_Tile_Shadows(int, Rect const &area)
{
    Point2D sample = Point2D(area.Top_Left()) + Point2D(TacPixelX, TacPixelY) - unknown_rect_839630.Top_Left();
    CoordStruct lepton;
    Set_Ground_Position(lepton, Unknown_006A2900(sample));
    if (lepton.Y < 0) {
        lepton.X -= lepton.Y;
        lepton.Y = 0;
    }
    if (lepton.X < 0) {
        lepton.Y -= lepton.X;
        lepton.X = 0;
    }

    Cell base = Map[lepton].Fetch_CellID();
    int ycount = area.Height / 15 + 20;
    int xcount = area.Width / 60 + 7;
    base.X -= 2;
    base.Y += 3;

    int ix, iy;
    for (iy = 0; iy < ycount; iy++) {
        Cell step(iy / 2, (iy + 1) / 2);
        Cell cell = base + step;

        for (ix = xcount; ix > 0; ix--) {
            if (Map.Unknown_0054AAB0(cell)) {
                Point2D pixel;
                Coord_To_Pixel(Coord_Whole(CoordStruct(cell.X * 256 + 128, cell.Y * 256 + 128, 0)), pixel);
                pixel.X -= 30;
                Map[cell].Draw_Shadow_Cast(pixel, area);
            }
            ++cell.X;
            --cell.Y;
        }
    }
}
