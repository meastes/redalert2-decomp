// void MapClass::Unknown_00555A60(Cell const &cell) (0x00555A60)
// TibSun MapClass::Repair_Bridge: the high bridge twin of Repair_Train_Bridge 0x00552570 (the second low bridge
// set 0x00562980 / 0x00561340, the bridge tile set 0x00A52C74, Repair_High_Bridge_Span 0x0054B560, and the span
// walk kept inside the map rectangle +0x120..+0x12C).
// The walk names its source cell through `Cell const &from` as the accepted twin 0x00557090 does: that one reference
// puts origin.Y first in the walk sum (`mov di,[esp+0x1a]`), and the walk-back step is Adjacent_Cell_L, offset left.
// @ref ?Unknown_00555A60@MapClass@@QAEXABVCell@@@Z=00555a60
// @ref ?Repair_High_Bridge_Span_0054b560@MapClass@@QAEXPAVCellClass@@W4FacingType@@PAVRect@@@Z=0054b560
// @ref ??0?$VectorClass@VCell@@@@QAE@IPBVCell@@@Z=0042d4a0
// @ref ??3@YAXPAX@Z=00781d3b
// @ref ??H?$TPoint2D@F@@QBE?BV0@ABV0@@Z=0042add0
// @ref ??_7?$DynamicVectorClass@VCell@@@@6B@=0079c5a0
// @ref ??_7?$VectorClass@VCell@@@@6B@=0079c5e0
// @ref ?AdjacentCell@@3QBVCell@@B=00851a70
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Set_Bridge_End_State@MapClass@@QAEXABVCell@@_N1@Z=00550eb0
// @ref ?Set_Bridge_Middle_State@MapClass@@QAEXABVCell@@HHH_N@Z=005510a0
// @ref ?Unknown_0054EBD0@MapClass@@QAEHXZ=0054ebd0
// @ref ?Unknown_00550230@MapClass@@QAE_NABVCell@@@Z=00550230
// @ref ?Unknown_005688A0@MapClass@@QAEXAAV?$DynamicVectorClass@VCell@@@@@Z=005688a0
// @ref ?unknown_dummy_cell_a6f9f8@@3VCellClass@@A=00a6f9f8
// @ref ?unknown_tactical_83996c@@3PAVTactical@@A=0083996c
// @ref ?unknown_tileset_a52c74@@3HA=00a52c74
// @ref ?unknown_tileset_a6cb7c@@3HA=00a6cb7c
// @ref ?unknown_tileset_a52e70@@3HA=00a52e70
// @ref ?unknown_tileset_a6e0e4@@3HA=00a6e0e4
// @ref ?unknown_tileset_a52f80@@3HA=00a52f80
// @ref ?unknown_tileset_a53394@@3HA=00a53394
// @ref ?unknown_tileset_a52588@@3HA=00a52588
#include "shared/dir_type.hpp"
#include "shared/map.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/map_adjacent.hpp"
#include "shared/unknown_globals.hpp"
#include "shared/tactical.hpp"
extern int unknown_tileset_a6e0e4;
extern int unknown_tileset_a52f80;
extern int unknown_tileset_a53394;
extern int unknown_tileset_a52588;

void MapClass::Unknown_00555A60(Cell const &cell)
{
    for (int x = -2; x < 3; x++) {
        for (int y = -2; y < 3; y++) {
            if (Unknown_00562980(cell + Cell(x, y))) {
                Repair_Low_Bridge2_Span(cell + Cell(x, y));
                return;
            }
        }
    }

    CellClass *cellptr = &Map[cell];
    int facing = FACING_N;
    if (!cellptr->IsUnderBridge && !cellptr->WasUnderBridge) {
        for (int index = 0; index < 8; index++) {
            Cell adjacent = Adjacent_Cell(cell, Facing_Add(facing, FACING_N));
            cellptr = &Map[adjacent];
            if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) {
                break;
            }
            adjacent = Adjacent_Cell(adjacent, Facing_Add(facing, FACING_N));
            cellptr = &Map[adjacent];
            if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) {
                break;
            }
            adjacent = Adjacent_Cell(adjacent, Facing_Add(facing, FACING_N));
            cellptr = &Map[adjacent];
            if (cellptr->IsUnderBridge || cellptr->WasUnderBridge) {
                break;
            }
            facing = Facing_Add(facing, FACING_NE);
        }
    }

    if (!cellptr->IsUnderBridge && !cellptr->WasUnderBridge) {
        return;
    }

    Cell origin;
    if (cellptr->IsUnderBridge) {
        origin = cellptr->Get_Bridge_Deck_Cell();
    } else {
        origin = cellptr->Fetch_CellID();
        int count = 0;
        int step;
        if (cellptr->IsBridgeEastWest) {
            step = FACING_S;
        } else {
            step = FACING_E;
        }
        CellClass *walkptr;
        while (true) {
            Cell const &from = origin;
            origin = Adjacent_Cell_L(from, step);
            walkptr = &Map[origin];
            if (walkptr->WasUnderBridge) {
                if (++count >= 4) {
                    return;
                }
            } else {
                break;
            }
        }
        FacingType back = Facing_Sub(step, FACING_S);
        origin = Adjacent_Cell_L(origin + AdjacentCell[back & 7], back);
    }

    int advance;
    if (cellptr->IsBridgeEastWest) {
        advance = FACING_W;
    } else {
        advance = FACING_N;
    }
    Cell spancell = origin;
    int ittype = 0;
    DynamicVectorClass<Cell> cells;
    char zonechanged = 0;
    Rect dirty;
    if (spancell.X >= unknown_120) {
    while (spancell.X >= unknown_120) {
        if (spancell.X > unknown_120 + unknown_128) {
            break;
        }
        if (spancell.Y < unknown_124 || spancell.Y > unknown_124 + unknown_12c) {
            break;
        }
        if (Has_Cell(spancell)) {
            CellClass *spanptr = &Map[spancell];
            Cell middle;
            ittype = spanptr->IsoTileIndex - unknown_tileset_a52c74 + 1;
            if ((ittype == unknown_tileset_a6e0e4 || ittype == unknown_tileset_a52f80) && spanptr->unknown_11a == 8) {
                Set_Bridge_End_State(spancell, false, false);
                Repair_High_Bridge_Span_0054b560(&Map[spancell], FACING_E, &dirty);
                unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                break;
            }
            if ((ittype == unknown_tileset_a6cb7c || ittype == unknown_tileset_a6cb7c + 3 ||
                 ittype == unknown_tileset_a6cb7c + 4 || ittype == unknown_tileset_a6cb7c + 1 ||
                 ittype == unknown_tileset_a6cb7c + 2) && spanptr->unknown_11a == 5) {
                if (ittype == unknown_tileset_a6cb7c + 4) {
                    Set_Bridge_Middle_State(spancell, unknown_tileset_a6cb7c + unknown_tileset_a52c74 - 1, -1, -1, false);
                    middle = Adjacent_Cell(spancell, FACING_W);
                    Map[middle].unknown_11b += 4;
                    Map[Adjacent_Cell(middle, FACING_N)].unknown_11b += 4;
                    Map[Adjacent_Cell(middle, FACING_S)].unknown_11b += 4;
                    zonechanged = Unknown_00550230(middle);
                    for (int dx = 0; dx < 2; dx++) {
                        for (int dy = -2; dy < 3; dy++) {
                            cells.Add(Cell(middle + Cell(dx, dy)));
                        }
                    }
                }
                Unknown_00555A60(spancell + Cell(-2, 0));
                Repair_High_Bridge_Span_0054b560(&Map[spancell], FACING_E, &dirty);
                unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                break;
            }
            if ((ittype == unknown_tileset_a53394 || ittype == unknown_tileset_a52588) && spanptr->unknown_11a == 12) {
                Set_Bridge_End_State(spancell, false, false);
                Repair_High_Bridge_Span_0054b560(&Map[spancell], FACING_S, &dirty);
                unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                break;
            }
            if ((ittype == unknown_tileset_a52e70 || ittype == unknown_tileset_a52e70 + 3 ||
                 ittype == unknown_tileset_a52e70 + 4 || ittype == unknown_tileset_a52e70 + 1 ||
                 ittype == unknown_tileset_a52e70 + 2) && spanptr->unknown_11a == 7) {
                if (ittype == unknown_tileset_a52e70 + 4) {
                    Set_Bridge_Middle_State(spancell, unknown_tileset_a52e70 + unknown_tileset_a52c74 - 1, -1, -1, false);
                    middle = Adjacent_Cell(spancell, FACING_N);
                    Map[middle].unknown_11b += 4;
                    Map[Adjacent_Cell(middle, FACING_E)].unknown_11b += 4;
                    Map[Adjacent_Cell(middle, FACING_W)].unknown_11b += 4;
                    zonechanged = Unknown_00550230(middle);
                    for (int dy = 0; dy < 2; dy++) {
                        for (int dx = -2; dx < 3; dx++) {
                            cells.Add(Cell(middle + Cell(dx, dy)));
                        }
                    }
                }
                Unknown_00555A60(spancell + Cell(0, -2));
                Repair_High_Bridge_Span_0054b560(&Map[spancell], FACING_S, &dirty);
                unknown_tactical_83996c->Register_Dirty_Area(dirty, false);
                break;
            }
        }
        spancell = Adjacent_Cell(spancell, advance);
    }
    }

    if (zonechanged) {
        Unknown_0054EBD0();
    }
    if (cells.Count() > 0) {
        Unknown_005688A0(cells);
    }
}
