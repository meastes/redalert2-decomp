// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void VeinholeMonsterClass::Draw_It() (0x00710880)
// from ra2-decomp 0x0074D490 (VeinholeMonsterClass::Draw_It, src/vein.cpp)
// Draws an unplaced monster: its shape frame (0 while the cell has flag
// 0x400000) at the cell centre, raised by the cell level, in the player's
// colours, and records the screen position with the tactical view.
#include "shared/veinhole.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/tactical.hpp"
#include "shared/palette.hpp"
#include "shared/color_scheme.hpp"
#include "shared/house.hpp"
#include "shared/terrain.hpp"
#include "shared/text_print.hpp"
// @ref ?unknown_veinhole_shape_ad079c@@3PBXB=00ad079c
// @ref ?unknown_tactical_83996c@@3PAVTactical@@A=0083996c
// @ref ?Add_To_Selectables@Tactical@@QAE_NPAVObjectClass@@UPoint2D@@@Z=006aa510

void VeinholeMonsterClass::Draw_It()
{
    if (!IsDead) {
        void const *shape = unknown_veinhole_shape_ad079c;
        int frame = ShapeFrame;
        CoordStruct coord = Cell_Coord(CellID);
        if (Map[coord].unknown_140 & 0x400000) {
            frame = 0;
        }
        Point2D pixel;
        unknown_tactical_83996c->Coord_To_Pixel(coord, pixel);
        int height = 15 * Map[CellID].unknown_11b;
        pixel.Y += -58 - height;
        unknown_tactical_83996c->Add_To_Selectables(this, Point2D(pixel.X, unknown_rect_839630.Y + pixel.Y + 58));
        ConvertClass *drawer = ColorSchemes[unknown_player_a35db4->ColorSchemeIndex]->Converter;
        CC_Draw_Shape(unknown_surface_83995c, drawer, (ShapeFileStruct const *)shape, frame, pixel, unknown_rect_839630, 0x2e00, 0, -17 - height, 0,
                      Map[coord].unknown_10c, 0, 0, Point2D(0, 0));
    }
}
