// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// void __stdcall TunnelLocomotionClass::Stop_Moving() (0x006EFDE0, ILocomotion 18)
// TibSun Tunnel.cpp Stop_Moving: a tunneling unit more than a cell diagonal from its destination
// heads for the nearest free cell (in its own zone first); with none it is destroyed.
// from ra2-decomp 0x00728C00 (TunnelLocomotionClass::Stop_Moving, src/tunnel.cpp)
#include "shared/tunnel_locomotion.hpp"
#include "shared/foot.hpp"
#include "shared/map_view.hpp"
#include "shared/rules.hpp"
#include "shared/unknown_geometry.hpp"

// The twin's Point2D::Length(): the double square root of the summed squares.
static inline int Point_Length(Point2D const &point)
{
    double y = point.Y;
    double x = point.X;
    return (int)unknown_sqrt_4ba410(x * x + y * y);
}

void __stdcall TunnelLocomotionClass::Stop_Moving()
{
    switch (unknown_18) {
    case 1:
        unknown_1c = unknown_coord_ac0f30;
        break;
    case 5:
        unknown_1c = unknown_coord_ac0f30;
        break;
    case 6:
        unknown_1c = unknown_coord_ac0f30;
        break;
    case 7:
        unknown_1c = unknown_coord_ac0f30;
        break;
    case 4: {
        CoordStruct pos = LinkedTo->Coord;
        if (Point_Length(Point2D(pos.X, pos.Y) - Point2D(unknown_1c.X, unknown_1c.Y)) > unknown_geometry_diagonal) {
            Cell cell = LinkedTo->unknown_slot104();
            Cell nearby = Map.Unknown_005502E0(cell, 1, Map.Zone(cell, 0, false), 0, false, Point2D(1, 1), 0, 0, 1, 1, Cell(0, 0), 0);
            if (nearby == unknown_geometry_k) {
                nearby = Map.Unknown_005502E0(cell, 1, -1, 0, false, Point2D(1, 1), 0, 0, 1, 1, Cell(0, 0), 0);
            }
            if (nearby == unknown_geometry_k) {
                LinkedTo->unknown_slot86(&LinkedTo->Strength, 0, Rules->unknown_c74, 0, true, true);
                unknown_1c = unknown_coord_ac0f30;
            } else {
                unknown_1c = Cell_Coord(nearby);
            }
        }
        break;
    }
    case 3:
        unknown_18 = 5;
        unknown_1c = unknown_coord_ac0f30;
        break;
    case 2:
        unknown_18 = 7;
        unknown_1c = unknown_coord_ac0f30;
        break;
    }
}
