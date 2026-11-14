// findpath2.cpp: the original translation unit re-filed as one source (Route A): the accepted
// bodies of its canonical compile group in address order, under the unit's own header
// state (its switches and includes in their original order). The module map
// manifests/original_target/modules/findpath2.json carries the addresses and the rows that
// stay in their own files.


#include "shared/cell_class.hpp"
#include "shared/bullet_type.hpp"
#include "shared/house.hpp"
#include "shared/map_view.hpp"
#include "shared/weapon_type.hpp"
#include "shared/cell.hpp"
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/overlay_type.hpp"
#include "shared/rules.hpp"


// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// From mischa85/RedAlert2 src/findpath2.cpp TrajectoryHelper::FindFirstObstacle (commit 5a79accc; matched there on
// this target); names adapted to this tree (Coord -> CoordStruct, GetObstacle -> Get_Obstacle_004bbaf0). See THIRD_PARTY.md.
// CellClass *Find_First_Obstacle_004bb890(...) (0x004BB890): walks the line from one coordinate to another one
// cell step at a time and returns the first cell that blocks the projectile type (a cliff rise or a wall when the
// type is subject to them, Get_Obstacle_004bbaf0), or null. The coordinate arithmetic is TibSun's TPoint3D
// (shared/coord.hpp, RA2_COORD_TPOINT3D + RA2_POINT3D_TIBSUN) and Coord::As_Cell (RA2_COORD_AS_CELL).

CellClass *__fastcall Get_Obstacle_004bbaf0(CellClass *source, CellClass *target, CellClass *last,
    CoordStruct coord, BulletTypeClass const *type, HouseClass const *house);

CellClass *__fastcall Find_First_Obstacle_004bb890(CoordStruct const & source, CoordStruct const & target, BulletTypeClass const * type, HouseClass const * owner)
{
	if (type->SubjectToCliffs || type->SubjectToWalls) {
		Cell scell = source.As_Cell();
		Cell tcell = target.As_Cell();
		int dx = abs(scell.X - tcell.X);
		int dy = abs(scell.Y - tcell.Y);
		int steps = dx;
		if (dx <= dy) {
			steps = dy;
		}
		CoordStruct delta = target - source;
		CoordStruct step;
		if (steps == 0) {
			step = CoordStruct(0, 0, 0);
		} else {
			step = CoordStruct(delta.X / steps, delta.Y / steps, delta.Z / steps);
		}
		CellClass * start = &Map[scell];
		CellClass * end = &Map[tcell];
		CellClass * prev = start;
		for (int i = 0; i < steps; i++) {
			CoordStruct offset = step * i;
			CoordStruct current = source + offset;
			CellClass * obstacle = Get_Obstacle_004bbaf0(start, end, prev, current, type, owner);
			if (obstacle != NULL) {
				return(obstacle);
			}
			prev = &Map[current.As_Cell()];
		}
	}
	return(NULL);
}


// CellClass *Find_First_Impenetrable_Obstacle_004bbaa0(...) (0x004BBAA0): the first cell on the line from
// one coordinate to another that blocks a weapon's projectile (Find_First_Obstacle_004bb890), unless the weapon's
// warhead destroys walls and the cell holds a wall or gate (names after Ares' hypotheses).

CellClass *__fastcall Find_First_Obstacle_004bb890(CoordStruct const &from, CoordStruct const &to,
    BulletTypeClass const *type, HouseClass const *house);

CellClass *__fastcall Find_First_Impenetrable_Obstacle_004bbaa0(CoordStruct const &from, CoordStruct const &to,
    WeaponTypeClass const *weapon, HouseClass const *house)
{
    BulletTypeClass const *type = weapon->Projectile;
    CellClass *cell = Find_First_Obstacle_004bb890(from, to, type, house);
    if (cell != 0 && cell->Has_Wall_Or_Gate(-1, -1) && weapon->Warhead->Wall) {
        return 0;
    }
    return cell;
}


// CellClass *Get_Obstacle_004bbaf0(...) (0x004BBAF0): the cell at coord when it blocks the projectile type (a cliff rise
// or a wall), unless coord lies near the cell's centre off the line's main axis and the cell passes the same tests again.
// Adapted from ra2-decomp src/findpath2.cpp TrajectoryHelper::GetObstacle (gamemd twin 0x004CC360, matched there);
// see THIRD_PARTY.md.
// Declaration-count sensitive: exact only with the SDK headers and the cell.h screen chain ahead of CellClass
// (RA2_SDK_HEADERS unit); margin measured with plain-prototype filler before cell_class.hpp: +2,048 still exact,
// +4,096 misses (97.00). On the fragile-probe list.

int __fastcall Cell_Height_Transition_004bbe10(CellClass const *origin, CellClass const *nearby, CellClass const *destination);
bool __fastcall Cell_Wall_Transition_004bbe60(CellClass const *origin, CellClass const *nearby, CellClass const *destination, HouseClass const *house);

CellClass *__fastcall Get_Obstacle_004bbaf0(CellClass *source, CellClass *target, CellClass *last,
    CoordStruct coord, BulletTypeClass const *type, HouseClass const *house)
{
    Cell cell = Cell(coord);
    CellClass *first = &Map[cell];
    if ((type->SubjectToCliffs && Cell_Height_Transition_004bbe10(source, last, first)) ||
        (type->SubjectToWalls && Cell_Wall_Transition_004bbe60(source, first, target, house))) {
        CoordStruct center = first->Center_Coord();
        if (coord.Distance_To(center) > 256 / 3) {
            CoordStruct from = source->Center_Coord();
            CoordStruct to = target->Center_Coord();
            int dx = abs(from.X - to.X);
            int dy = abs(from.Y - to.Y);
            int ox = abs(coord.X - center.X);
            int oy = abs(coord.Y - center.Y);
            if ((dx > dy && oy > ox) || (dy > dx && ox > oy)) {
                CellClass *check = &Map[cell];
                if (type->SubjectToCliffs && Cell_Height_Transition_004bbe10(source, last, check)) {
                    return first;
                }
                if (type->SubjectToWalls && Cell_Wall_Transition_004bbe60(source, check, target, house)) {
                    return first;
                }
                return 0;
            }
            return first;
        }
        return first;
    }
    return 0;
}


// The cliff test of the projectile path (inlined into Get_Obstacle_004bbaf0, as the gamemd twin
// TrajectoryHelper::IsCliffHit in ra2-decomp src/findpath2.cpp; see THIRD_PARTY.md): the cell rises four or more
// levels over the previous cell and above the origin. CellClass 0x0047CF10 returns +0x11B plus four for flag 0x80.
int __fastcall Cell_Height_Transition_004bbe10(CellClass const *origin,
                                             CellClass const *nearby,
                                             CellClass const *destination)
{
    return destination->Bridge_Adjusted_Height_0047CF10() - nearby->Bridge_Adjusted_Height_0047CF10() >= 4
        && destination->Bridge_Adjusted_Height_0047CF10() - origin->Bridge_Adjusted_Height_0047CF10() > 0;
}


bool __fastcall Cell_Wall_Transition_004bbe60(CellClass const *origin,
    CellClass const *nearby, CellClass const *destination, HouseClass const *house)
{
    bool wall = nearby->OverlayIndex != -1 &&
        OverlayTypes[nearby->OverlayIndex]->Wall;
    if (nearby != destination && wall && origin->unknown_11b <= destination->unknown_11b) {
        if (Rules->unknown_1498) {
            HouseClass *owner=Houses[nearby->unknown_50];
            if (owner && owner->Is_Ally(house)) return false;
        }
        return true;
    }
    return false;
}
