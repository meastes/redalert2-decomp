// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// UnitClass::Desired_Load_Dir (0x00704730, slot 182), from ra2-decomp UnitClass::Desired_Load_Dir
// 0x00740B60 (YR twin). Scores the eight neighbouring cells against the direction to the
// passenger (or the reverse of the body facing): +128 for a cell the passenger may enter (or
// already occupies), or, without a passenger, for passable land with no building, vehicle or
// monolith and no unallied occupant; -128 otherwise; less the angular distance, less 100 for
// FACING_S. Stores the best cell in *cell (the null cell when none scores above 0) and answers
// its facing from Harvester_Load_List (0x007F8F4C; FACING_S by default); trains keep their body
// facing.
#include "shared/unit.hpp"
#include "shared/cell.hpp"
#include "shared/house.hpp"
#include "shared/map_view.hpp"
#include "shared/terrain.hpp"
#include "shared/dir_type.hpp"
#include "shared/incdec.hpp"
#include "shared/house_adjacent.hpp"
// @ref ?Direction@UnknownFacingDir@@QAEAAU1@ABUCoordStruct@@0@Z=00423ef0
// @ref ?Current@FacingClass@@QBE?AUUnknownFacingDir@@XZ=004b8be0
// @ref ?AdjacentCell@@3QBVCell@@B=00851a70
// @ref ?Ground@@3PAUGroundType@@A=00850ea0
// @ref ?Is_Ally@HouseClass@@QBE_NPBVTechnoClass@@@Z=004e5580
// @ref ?unknown_facings_7f8f4c@@3QBHB=007f8f4c
// @ref ?unknown_5d47d0@ObjectClass@@QAEHXZ=005d47d0
// @ref __real@4@00000000000000000000=0079a738
// @ref _unknown_geometry_k=00ad0488

// CELL_NONE (0x00AD0488; as UnitClass slot 55).
// Harvester_Load_List: the facing answered for each exit (0x007F8F4C).
extern int const unknown_facings_7f8f4c[8];

// The twin's file-local Direction wrapper (DirType::Direction 0x00423EF0, called out of line).
static inline UnknownFacingDir &_Direction(UnknownFacingDir &dir, ObjectClass *from, AbstractClass *to)
{
    return dir.Direction(from->Center_Coord(), to->Center_Coord());
}

int UnitClass::unknown_slot182(AbstractClass *object, Cell *cell)
{
    FacingType face = FACING_N;
    int faceto;
    if (object) {
        UnknownFacingDir direction = _Direction(direction, this, object);
        faceto = Dir_To_256(direction);
    } else {
        UnknownFacingDir dir((unsigned short)(unknown_2e0.Current().Raw + 0x7FFF));
        faceto = Dir_To_256(dir);
    }
    int desired = (signed char)faceto;
    FacingType bestdir = FACING_N;
    int bestval = -1;
    for (; face < FACING_COUNT; face++) {
        int value = 0;
        Cell cellnum = Adjacent_Cell(unknown_slot104(), face);
        if (object) {
            value = (!((ObjectClass *)object)->Can_Enter_Cell(&Map[cellnum], face, unknown_5d47d0(), 0, 1)
                || ((ObjectClass *)object)->unknown_slot104() == cellnum) ? 128 : -128;
        } else {
            CellClass *land = &Map[Cell(cellnum)];
            if (Ground[land->unknown_ec].Cost[0] == 0.0f || land->Occupy_124.Building || land->Occupy_124.Vehicle || land->Occupy_124.Monolith
                || (land->unknown_124 & 0x1F) == 0x1F) {
                value = -128;
            } else {
                if (land->Cell_Techno(Point2D(0, 0), false, 0)
                    && !House->Is_Ally((TechnoClass *)land->Cell_Techno(Point2D(0, 0), false, 0))) {
                    value = -128;
                } else {
                    value = 128;
                }
            }
        }
        int here = DirType(face).As_Dir256();
        int diff = (signed char)here - desired;
        value -= abs(diff);
        if (face == FACING_S) {
            value -= 100;
        }
        if (bestval == -1 || value > bestval) {
            bestval = value;
            bestdir = face;
        }
    }
    int facing = FACING_S;
    *cell = unknown_geometry_k;
    if (bestval > 0) {
        *cell = Adjacent_Cell(unknown_slot104(), bestdir);
        facing = unknown_facings_7f8f4c[bestdir];
    }
    if (Type->IsTrain) {
        facing = Dir_To_8(unknown_2e0.Current());
    }
    return facing;
}
