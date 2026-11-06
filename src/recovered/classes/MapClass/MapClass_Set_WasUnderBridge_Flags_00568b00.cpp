// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void MapClass::Set_WasUnderBridge_Flags() (0x00568B00), from ra2-decomp MapClass::Set_WasUnderBridge_Flags
// (YR 0x00586BF0, src/map.cpp; layout twin): every impassable bridge connection (type 0) walks from
// its From cell to its To cell; each spanned cell that is not itself under the bridge marks a four-cell
// strip across it as having been under a bridge, with the strip's direction in IsBridgeEastWest (set
// for a bridge running east/west). The twin's file-local Bridge_Start_Cell helper takes the second
// branch's first step (one more inline level than the first branch's direct Adjacent_Cell).
#include "shared/map.hpp"
#include "shared/map_cell.hpp"
#include "shared/map_adjacent.hpp"

static inline Cell Bridge_Start_Cell(Cell const &cell, FacingType facing)
{
    return Adjacent_Cell(cell, facing);
}

void MapClass::Set_WasUnderBridge_Flags()
{
    for (int index = ZoneConnections.Count() - 1; index >= 0; index--) {
        ZoneConnectionClass *con = &ZoneConnections[index];

        if (con->unknown_0c == 0 && !con->unknown_08) {
            if (con->From.X == con->To.X) {
                FacingType facing = con->From.Y < con->To.Y ? FACING_S : FACING_N;
                Cell cell = Adjacent_Cell(con->From, facing);
                for (; cell != con->To; cell = cell + AdjacentCell[(int)facing & 7]) {
                    if (!Map[cell].IsUnderBridge) {
                        for (int offset = -2; offset <= 1; offset++) {
                            CellClass &cptr = Map[cell + Cell(offset, 0)];
                            cptr.IsBridgeEastWest = false;
                            cptr.WasUnderBridge = true;
                        }
                    }
                }
            } else {
                FacingType facing = con->From.X < con->To.X ? FACING_E : FACING_W;
                Cell cell = Bridge_Start_Cell(con->From, facing);
                for (; cell != con->To; cell = AdjacentCell[(int)facing & 7] + cell) {
                    if (!Map[cell].IsUnderBridge) {
                        for (int offset = -2; offset <= 1; offset++) {
                            CellClass &cptr = Map[cell + Cell(0, offset)];
                            cptr.IsBridgeEastWest = true;
                            cptr.WasUnderBridge = true;
                        }
                    }
                }
            }
        }
    }
}
