// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// Cell DisplayClass::Calculated_Cell(int dir, Cell const &waypoint, Cell const &cell, int loco, bool zonecheck, int mzone) (0x0049A700)
// TibSun DisplayClass::Calculated_Cell: a reinforcement cell on the map edge nearest the waypoint
// (else the cell, else a random point on edge dir); the south edge collects candidates and picks the
// closest. From ra2-decomp DisplayClass::Calculated_Cell (YR 0x004AA440, src/display.cpp), adapted to
// our names (RA2 has no In_Local_Radar test on the west/east and north edges).
#include "shared/display.hpp"
#include "shared/map_view.hpp"
#include "shared/id_source.hpp"
#include "shared/always.hpp"
#include "shared/fast_math.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?unknown_cell_852610@@3VCell@@A=00852610
// @ref ?unknown_id_source_a3d290@@3PAVUnknownIDSource@@A=00a3d290
// @ref ??R?$Random2Class@@QAEHHH@Z=006388a0
// @ref ?Zone@MapClass@@QAEHABVCell@@H_N@Z=0054f8f0
// @ref ?Unknown_00547D70@MapClass@@QAE?AUPoint2D@@ABU2@@Z=00547d70
// @ref ?Unknown_00547DF0@MapClass@@QAE?AUPoint2D@@ABU2@@Z=00547df0
// @ref ?Unknown_0055A660@MapClass@@QAE_NABVCell@@_N@Z=0055a660
// @ref ?Unknown_0049AD90@DisplayClass@@QAE_NABVCell@@0HHH@Z=0049ad90
// @ref ?Fast_Sqrt@@YAMN@Z=004ba410
// @ref ??0?$VectorClass@VCell@@@@QAE@IPBVCell@@@Z=0042d4a0
// @ref ??_7?$VectorClass@VCell@@@@6B@=0079c5e0
// @ref ??_7?$DynamicVectorClass@VCell@@@@6B@=0079c5a0
// @ref ??_G?$VectorClass@VCell@@@@UAEPAXI@Z=0042d390
// @ref ??_E?$VectorClass@VCell@@@@UAEPAXI@Z=0042d390
// @ref ??_G?$DynamicVectorClass@VCell@@@@UAEPAXI@Z=0042d3e0
// @ref ??_E?$DynamicVectorClass@VCell@@@@UAEPAXI@Z=0042d3e0
// @ref ??8?$VectorClass@VCell@@@@UBE_NABV0@@Z=0042ce10
// @ref ?Resize@?$VectorClass@VCell@@@@UAE_NHPBVCell@@@Z=0042cee0
// @ref ?Resize@?$DynamicVectorClass@VCell@@@@UAE_NHPBVCell@@@Z=0042d050
// @ref ?Clear@?$VectorClass@VCell@@@@UAEXXZ=0042cfb0
// @ref ?Clear@?$DynamicVectorClass@VCell@@@@UAEXXZ=0042cc10
// @ref ?ID@?$VectorClass@VCell@@@@UAEHABVCell@@@Z=0042cfe0
// @ref ?ID@?$VectorClass@VCell@@@@UAEHPBVCell@@@Z=0042d030
// @ref ?ID@?$DynamicVectorClass@VCell@@@@UAEHABVCell@@@Z=0042d120
// @ref ?ID@?$DynamicVectorClass@VCell@@@@UAEHPBVCell@@@Z=0042cc40
// @ref ?Fetch@?$VectorClass@VCell@@@@UBE?AVCell@@H@Z=0042cc60


Cell DisplayClass::Calculated_Cell(int dir, Cell const &waypoint, Cell const &cell, int loco, bool zonecheck, int mzone)
{
    bool vert = false;
    bool horz = false;
    int x;
    int y;
    int start;
    Cell fallback;
    int zone = -1;

    Cell trycell(0, 0);
    if (waypoint != unknown_cell_852610) {
        trycell = waypoint;
    }
    if (trycell == unknown_cell_852610) {
        trycell = cell;
    }

    if (zonecheck && trycell != unknown_cell_852610) {
        zone = Map.Zone(trycell, mzone, true);
    }

    Point2D trypoint = Unknown_00547DF0(Point2D(trycell.X, trycell.Y));

    if (trycell != unknown_cell_852610) {
        int left = trypoint.X - MapLocalSize.X;
        x = (left < MapLocalSize.X + (MapLocalSize.Width - trypoint.X)
            ? left : MapLocalSize.X + (MapLocalSize.Width - trypoint.X)) * 2;
        y = trypoint.Y - 2 * MapLocalSize.Y;
        y = min(y, -trypoint.Y + 2 * (MapLocalSize.Y + MapLocalSize.Height));

        if (x < y) {
            vert = true;
            horz = false;
            if ((trypoint.X - MapLocalSize.X) < MapLocalSize.Width / 2) {
                x = -1;
                dir = 3;
            } else {
                x = MapLocalSize.Width;
                dir = 1;
            }
            y = 0;
            start = trypoint.Y - MapLocalSize.Y;
        } else {
            vert = false;
            horz = true;
            if ((trypoint.Y - 2 * MapLocalSize.Y) < (2 * MapLocalSize.Height) / 2) {
                y = -1;
                dir = 0;
            } else {
                y = 2 * MapLocalSize.Height;
                dir = 2;
            }
            x = 0;
            start = trypoint.X - MapLocalSize.X;
        }
    } else {
        y = x = 0;
        switch (dir) {
            default:
            case 0:
                horz = true;
                start = Random_Pick(1, MapLocalSize.Width) - 1;
                break;

            case 2:
                horz = true;
                y = 2 * MapLocalSize.Height + 2;
                start = Random_Pick(1, MapLocalSize.Width) - 1;
                break;

            case 1:
                vert = true;
                x = MapLocalSize.Width;
                start = Random_Pick(1, 2 * MapLocalSize.Height) - 1;
                break;

            case 3:
                vert = true;
                start = Random_Pick(0, 2 * MapLocalSize.Height) - 1;
                break;
        }
    }

    Point2D defaultPoint = Unknown_00547D70(Point2D(1, MapLocalSize.Width / 2));
    fallback = Cell(defaultPoint.X, defaultPoint.Y);

    if (vert) {
        int modifier = (dir != 1) ? 1 : -1;

        for (int index = 0; index < MapLocalSize.Height * 2; index++) {
            int yy = (start + index) % (MapLocalSize.Height * 2);

            Point2D outcellPoint = Unknown_00547D70(Point2D(x, yy));
            Cell outcell(outcellPoint.X, outcellPoint.Y);
            Point2D incellPoint = Unknown_00547D70(Point2D(x + modifier, yy));

            if (Unknown_0049AD90(outcell, Cell(incellPoint.X, incellPoint.Y), loco, zone, mzone)) {
                return outcell;
            }
        }
    } else if (horz) {
        int modifier = (dir == 0) ? 1 : -1;

        if (dir == 2) {
            DynamicVectorClass<Cell> cells;

            for (x = 0; x < MapLocalSize.Width; x++) {
                for (y = 0; y < 15; y++) {
                    Point2D newcellPoint = Unknown_00547D70(Point2D(x, y + 2 * MapLocalSize.Height));
                    Cell newcell(newcellPoint.X, newcellPoint.Y);
                    if (!Unknown_0055A660(newcell, true) && Unknown_0049AD90(newcell, trycell, loco, zone, mzone)) {
                        cells.Add(newcell);
                        break;
                    }
                }
            }
            int count = cells.Count();

            if (trycell == unknown_cell_852610) {
                if (count > 0) {
                    fallback = cells[Random_Pick(0, count - 1)];
                } else {
                    fallback = unknown_cell_852610;
                }
            } else {
                int dist = -1;
                Cell best = unknown_cell_852610;
                for (int i = 0; i < count; i++) {
                    int d = trycell.Distance_To(cells[i]);
                    if (dist == -1 || d < dist) {
                        best = cells[i];
                        dist = d;
                    }
                }
                fallback = best;
            }

        } else {
            for (int index = 0; index < MapLocalSize.Width; index++) {
                int xx = (start + index) % MapLocalSize.Width;

                Point2D outcellPoint = Unknown_00547D70(Point2D(xx, y));
                Cell outcell(outcellPoint.X, outcellPoint.Y);
                Point2D incellPoint = Unknown_00547D70(Point2D(xx, y + modifier));

                if (Unknown_0049AD90(outcell, Cell(incellPoint.X, incellPoint.Y), loco, zone, mzone)) {
                    return outcell;
                }
            }
        }
    }

    return fallback;
}
