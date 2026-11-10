// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright 2026 ra2-decomp contributors
// Copyright 2025 Electronic Arts Inc.; 2026 OpenTS contributors
// Derived from Electronic Arts source code; EA's GPL Section 7 terms apply (COPYING.md).
// UnitClass::Plan_Route (0x00707F60), from ra2-decomp UnitClass::Plan_Route 0x007447B0 (YR twin).
// Split a subterranean vehicle trip at bridge ends and reachable surface cells.
#include "shared/unit.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/map_view.hpp"
#include "shared/map_elements.hpp"
#include "shared/cell.hpp"
// @ref ?Unknown_005651E0@MapClass@@=005651e0
// @ref ?Nearby_Location@MapClass@@=005502e0
// @ref ?Zone@MapClass@@=0054f8f0
// @ref ?Unknown_0054F7C0@MapClass@@=0054f7c0
// @ref ?unknown_47c200@CellClass@@=0047c200
// @ref ?unknown_709790@UnitClass@@=00709790

AbstractClass *UnitClass::Plan_Route_00707f60(AbstractClass *target)
{

    unknown_478.Clear();
    bool leaving_bridge=unknown_slot46(0);
    Cell origin=Cell(unknown_slot19(0));
    bool origin_burrow=Map[origin].unknown_47c200() || unknown_slot108()<0;
    Cell goal=Cell(target->Center_Coord());
    CellClass *goal_cell=&Map[goal];
    bool goal_bridge=(goal_cell->unknown_140&0x100)!=0;
    bool goal_burrow=goal_cell->unknown_47c200();
    if (leaving_bridge) {
        if (goal_bridge) {
            Cell departure=Map.Unknown_005651E0(origin,goal);
            Cell arrival=Map.Unknown_005651E0(goal,goal);
            if (unknown_709790(&departure,&arrival)) {
                Cell first=Map.Nearby_Location(departure,1,Map.Zone(departure,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
                Cell last=Map.Nearby_Location(arrival,1,Map.Zone(departure,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
                if (first!=unknown_geometry_k && last!=unknown_geometry_k) {
                    unknown_478.Add(&Map[first]);
                    unknown_478.Add(&Map[last]);
                } else if (Map.Unknown_0054F7C0(origin,goal,0,true,true,false)) {
                    if (departure!=origin) unknown_478.Add(&Map[departure]);
                    unknown_478.Add(&Map[arrival]);
                } else if (first!=unknown_geometry_k) {
                    last=Map.Nearby_Location(goal,1,-1,0,true,Point2D(1,1),0,0,1,1,Cell(0,0),0);
                    if (last!=unknown_geometry_k && first!=last) {
                        unknown_478.Add(&Map[first]);
                        target=&Map[last];
                    } else {
                        Cell alternative=Map.Nearby_Location(goal,1,Map.Zone(origin,0,true),0,true,Point2D(1,1),0,1,0,1,Cell(0,0),0);
                        if (alternative==unknown_geometry_k || unknown_slot108()<0) return 0;
                        target=&Map[alternative];
                    }
                } else {
                    Cell alternative=Map.Nearby_Location(goal,1,Map.Zone(origin,0,true),0,true,Point2D(1,1),0,1,0,1,Cell(0,0),0);
                    if (alternative==unknown_geometry_k || unknown_slot108()<0) return 0;
                    target=&Map[alternative];
                }
            } else {
                if (departure!=origin) unknown_478.Add(&Map[departure]);
                unknown_478.Add(&Map[arrival]);
            }
        } else if (goal_burrow) {
            Cell departure=Map.Unknown_005651E0(origin,goal);
            if (unknown_709790(&departure,&goal)) {
                Cell first=Map.Nearby_Location(departure,1,Map.Zone(departure,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
                if (first!=unknown_geometry_k) {
                    unknown_478.Add(&Map[first]);
                } else if (Map.Unknown_0054F7C0(origin,goal,0,true,false,false)) {
                    if (departure!=origin) unknown_478.Add(&Map[departure]);
                } else {
                    Cell alternative=Map.Nearby_Location(goal,1,Map.Zone(origin,0,true),0,true,Point2D(1,1),0,1,0,1,Cell(0,0),0);
                    if (alternative==unknown_geometry_k || unknown_slot108()<0) return 0;
                    target=&Map[alternative];
                }
            } else if (departure!=origin) unknown_478.Add(&Map[departure]);
        } else {
            Cell departure=Map.Unknown_005651E0(origin,goal);
            if (unknown_709790(&departure,&goal)) {
                Cell first=Map.Nearby_Location(departure,1,Map.Zone(departure,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
                Cell last=Map.Nearby_Location(goal,1,Map.Zone(goal,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
                if (first!=unknown_geometry_k && last!=unknown_geometry_k) {
                    unknown_478.Add(&Map[first]);
                    unknown_478.Add(&Map[last]);
                } else if (Map.Unknown_0054F7C0(origin,goal,0,true,false,false)) {
                    if (departure!=origin) unknown_478.Add(&Map[departure]);
                } else if (first!=unknown_geometry_k) {
                    last=Map.Nearby_Location(goal,1,-1,0,true,Point2D(1,1),0,1,1,1,Cell(0,0),0);
                    if (last!=unknown_geometry_k && last!=first) {
                        unknown_478.Add(&Map[first]);
                        target=&Map[last];
                    } else {
                        Cell replacement=Map.Nearby_Location(goal,1,Map.Zone(origin,0,true),0,true,Point2D(1,1),0,1,0,1,Cell(0,0),0);
                        if (replacement==unknown_geometry_k || unknown_slot108()<0) return 0;
                        target=&Map[replacement];
                    }
                } else {
                    Cell alternative=Map.Nearby_Location(goal,1,Map.Zone(origin,0,true),0,true,Point2D(1,1),0,1,0,1,Cell(0,0),0);
                    if (alternative==unknown_geometry_k || unknown_slot108()<0) return 0;
                    target=&Map[alternative];
                }
            } else if (departure!=origin) unknown_478.Add(&Map[departure]);
        }
    } else if (origin_burrow) {
        if (goal_bridge) {
            Cell arrival=Map.Unknown_005651E0(goal,goal);
            if (unknown_709790(&origin,&arrival)) {
                Cell last=Map.Nearby_Location(arrival,1,Map.Zone(arrival,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
                if (last!=unknown_geometry_k) {
                    unknown_478.Add(&Map[last]);
                } else if (Map.Unknown_0054F7C0(origin,goal,0,false,true,false)) {
                    if (arrival!=origin) unknown_478.Add(&Map[arrival]);
                } else {
                    last=Map.Nearby_Location(goal,1,-1,0,false,Point2D(1,1),0,0,1,1,Cell(0,0),0);
                    if (last!=unknown_geometry_k) {
                        target=&Map[last];
                    } else {
                        Cell replacement=Map.Nearby_Location(goal,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,1,0,1,Cell(0,0),0);
                        if (replacement==unknown_geometry_k || unknown_slot108()<0) return 0;
                        target=&Map[replacement];
                    }
                }
            } else if (arrival!=origin) unknown_478.Add(&Map[arrival]);
        } else if (!goal_burrow && unknown_709790(&origin,&goal)) {
            Cell last=Map.Nearby_Location(goal,1,Map.Zone(goal,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
            if (last!=unknown_geometry_k) {
                unknown_478.Add(&Map[last]);
            } else if (!Map.Unknown_0054F7C0(origin,goal,0,true,true,false)) {
                last=Map.Nearby_Location(goal,1,-1,0,false,Point2D(1,1),0,0,1,1,Cell(0,0),0);
                if (last!=unknown_geometry_k) {
                    target=&Map[last];
                } else {
                    Cell replacement=Map.Nearby_Location(goal,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,0,0,1,Cell(0,0),0);
                    if (replacement==unknown_geometry_k || unknown_slot108()<0) return 0;
                    target=&Map[replacement];
                }
            }
        }
    } else if (goal_bridge) {
        Cell arrival=Map.Unknown_005651E0(goal,goal);
        if (unknown_709790(&origin,&arrival)) {
            Cell first=Map.Nearby_Location(origin,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
            Cell last=Map.Nearby_Location(arrival,1,Map.Zone(arrival,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
            if (first!=unknown_geometry_k && last!=unknown_geometry_k) {
                unknown_478.Add(&Map[first]);
                unknown_478.Add(&Map[last]);
            } else if (Map.Unknown_0054F7C0(origin,goal,0,false,true,false)) {
                if (arrival!=origin) unknown_478.Add(&Map[arrival]);
            } else if (first!=unknown_geometry_k) {
                last=Map.Nearby_Location(goal,1,-1,0,false,Point2D(1,1),0,0,1,1,Cell(0,0),0);
                if (last!=unknown_geometry_k && first!=last) {
                    unknown_478.Add(&Map[first]);
                    target=&Map[last];
                } else {
                    Cell replacement=Map.Nearby_Location(goal,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,0,0,1,Cell(0,0),0);
                    if (replacement==unknown_geometry_k || unknown_slot108()<0) return 0;
                    target=&Map[replacement];
                }
            } else {
                Cell alternative=Map.Nearby_Location(goal,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,0,0,1,Cell(0,0),0);
                if (alternative==unknown_geometry_k || unknown_slot108()<0) return 0;
                target=&Map[alternative];
            }
        } else if (arrival!=origin) unknown_478.Add(&Map[arrival]);
    } else if (goal_burrow) {
        if (unknown_709790(&origin,&goal)) {
            Cell first=Map.Nearby_Location(origin,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
            if (first!=unknown_geometry_k) {
                unknown_478.Add(&Map[first]);
            } else if (!Map.Unknown_0054F7C0(origin,goal,0,false,false,false)) {
                Cell alternative=Map.Nearby_Location(goal,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,0,0,1,Cell(0,0),0);
                if (alternative==unknown_geometry_k || unknown_slot108()<0) return 0;
                target=&Map[alternative];
            }
        }
    } else if (unknown_709790(&origin,&goal)) {
        Cell first=Map.Nearby_Location(origin,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
        Cell last=Map.Nearby_Location(goal,1,Map.Zone(goal,0,false),0,false,Point2D(1,1),0,1,1,1,Cell(0,0),0);
        if (first!=unknown_geometry_k && last!=unknown_geometry_k) {
            unknown_478.Add(&Map[first]);
            unknown_478.Add(&Map[last]);
        } else if (!Map.Unknown_0054F7C0(origin,goal,0,true,true,false)) {
            if (first!=unknown_geometry_k) {
                last=Map.Nearby_Location(goal,1,-1,0,false,Point2D(1,1),0,0,1,1,Cell(0,0),0);
                if (last!=unknown_geometry_k && first!=last) {
                    unknown_478.Add(&Map[first]);
                    target=&Map[last];
                } else {
                    Cell replacement=Map.Nearby_Location(goal,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,0,0,1,Cell(0,0),0);
                    if (replacement==unknown_geometry_k || unknown_slot108()<0) return 0;
                    target=&Map[replacement];
                }
            } else {
                Cell alternative=Map.Nearby_Location(goal,1,Map.Zone(origin,0,false),0,false,Point2D(1,1),0,0,0,1,Cell(0,0),0);
                if (alternative==unknown_geometry_k || unknown_slot108()<0) return 0;
                target=&Map[alternative];
            }
        }
    }
    unknown_478.Add(target);
    AbstractClass *next=unknown_478[0];
    unknown_478.Delete(0);
    return next;
}
