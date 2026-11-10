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
#include "shared/cell_class.hpp"
#include "shared/bullet_type.hpp"
#include "shared/house.hpp"
#include "shared/map_view.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ??AMapClass@@QAEAAVCellClass@@ABVCell@@@Z=00548070
// @ref ?Get_Obstacle_004bbaf0@@YIPAVCellClass@@PAV1@00UCoordStruct@@PBVBulletTypeClass@@PBVHouseClass@@@Z=004bbaf0

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
