// SPDX-License-Identifier: GPL-3.0-or-later
// Shape adopted from ra2-decomp (gibbo101/ra2-decomp ca9b8ad, src/techno.cpp TechnoClass::Get_Z_Fudge_Tunnel,
// YR 0x00704000, and include/inline.h's Dir_Facing/Adjacent_Cell): Copyright 2026 ra2-decomp contributors;
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors. Derived from Electronic Arts source code;
// EA's GPL Section 7 terms apply.
// int TechnoClass::unknown_6cfb60() (0x006CFB60)
// Whether a tunnel entrance lies two cells to the north or west of the
// object (1) or not (0), for the drawing depth (times ZFudgeTunnel).
#include "shared/techno.hpp"
#include "shared/gscreen.hpp"
#include "shared/foot.hpp"
#include "shared/techno_map.hpp"
#include "shared/cell.hpp"
#include "shared/infantry.hpp"
// @ref ?AdjacentCell@@3QBVCell@@B=00851a70

#include "shared/dir_type.hpp"

extern Cell const AdjacentCell[8];

// TibSun inline.h's Dir_Facing and the Dir256 Adjacent_Cell (the cell is the sum's left operand).
inline FacingType Dir_Facing(DirType facing) { return facing.As_Facing(); }
inline FacingType Dir_Facing(Dir256 facing) { return Dir_Facing(DirType(facing)); }
inline Cell Adjacent_Cell(Cell const &cell, Dir256 dir) { return cell + AdjacentCell[Dir_Facing(dir)]; }

int TechnoClass::unknown_6cfb60()
{
    int fudge = 0;
    Cell cell = unknown_slot104();
    if (!unknown_79) {
        CellClass *north = &Map[Adjacent_Cell(cell, DIR_N)];
        CellClass *west = &Map[Adjacent_Cell(cell, DIR_W)];
        Cell tuncell = unknown_cell_ac0148;
        CellClass *own = &Map[cell];
        if (north != 0 && west != 0) {
            if (own->Has_Tunnel()) {
                tuncell = own->Fetch_CellID();
            } else if (north->Has_Tunnel()) {
                tuncell = north->Fetch_CellID();
            } else if (west->Has_Tunnel()) {
                tuncell = west->Fetch_CellID();
            }
            if (tuncell != unknown_cell_ac0148) {
                north = &Map[Adjacent_Cell(tuncell, DIR_N)];
                west = &Map[Adjacent_Cell(tuncell, DIR_W)];
                if (north != 0 && west != 0) {
                    north = &Map[Adjacent_Cell(north->Fetch_CellID(), DIR_N)];
                    west = &Map[Adjacent_Cell(west->Fetch_CellID(), DIR_W)];
                    if (north != 0 && west != 0) {
                        if (west->Has_Tunnel() || north->Has_Tunnel()) {
                            fudge = 1;
                        }
                    }
                }
            }
        }
    }
    return fudge;
}
