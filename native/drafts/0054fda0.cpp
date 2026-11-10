// NON-EXACT DRAFT 0x54FDA0 FUN_0054fda0
// entry 0x54FDA0; exact-match score 60.63 (misses_index best); plan "review: rewrite likely"
// origin lane: Scout1027 (build/class-work/Scout1027/keep/map_zone_connections_54fda0_6063.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1069 (native/drafts.json). Not original bytes.
// void MapClass::Compute_Zone_Connections() (0x0054FDA0)
// TibSun Compute_Zone_Connections: rebuilds ZoneConnections (+0x4C) from a walk over every
// cell. A tunnel cell with tunnel cells on both sides (east and west, or south and north)
// records one connection to its tube's exit, from the lower numbered cell of the pair
// (type 1). A bridge or train bridge cell at the starting subtile of its span walks along
// the span's facing until it passes the far end subtile, then records a connection to the
// cell before the end (type 0), passable unless a cell on the way was neither a bridge nor
// under one; a walk that leaves the map first records nothing.
#include "shared/map.hpp"
#include "shared/map_cell.hpp"
#include "shared/tileset_index.hpp"
#include "shared/tube.hpp"
// The isometric cell number of a cell (0x00428A90; declared as in shared/veinhole.hpp).
int __fastcall unknown_cell_index_428a90(Cell const &cell);
// @ref ?unknown_cell_index_428a90@@YIHABVCell@@@Z=00428a90
// @ref ?Get_Tunnel@CellClass@@QAEPAVTubeClass@@XZ=0047a130
// @ref _?_bridge_subtiles1@?1??Compute_Zone_Connections@MapClass@@QAEXXZ@4PAHA=007e0244
// @ref _?_facings@?1??Compute_Zone_Connections@MapClass@@QAEXXZ@4PAHA=007e0284
// @ref _?_bridge_subtiles2@?1??Compute_Zone_Connections@MapClass@@QAEXXZ@4PAHA=007e02c4

void MapClass::Compute_Zone_Connections()
{
    static int _bridge_subtiles1[] = {
        7, 7, -1, 7, 7, -1, 4, 4,
        4, 4,  4, 2, 2,  2, 2, 2
    };
    static int _facings[] = {
        2, 2, -1, 4, 4, -1, 2, 2,
        2, 2,  2, 4, 4,  4, 4, 4
    };
    static int _bridge_subtiles2[] = {
        -1, -1, 4, -1, -1, 2, 4, 4,
         4,  4, 4,  2,  2, 2, 2, 2
    };

    ZoneConnections.Clear();
    Reset_Iterator();
    CellClass *cptr = Iterate();

    while (cptr != 0) {

        if (!cptr->unknown_47b960() && !cptr->unknown_47b980()) {

            if (cptr->Has_Tunnel()) {
                if ((cptr->Adjacent_Cell(2).Has_Tunnel() && cptr->Adjacent_Cell(6).Has_Tunnel()) ||
                    (cptr->Adjacent_Cell(4).Has_Tunnel() && cptr->Adjacent_Cell(0).Has_Tunnel())) {

                    Cell cell = cptr->Get_Tunnel()->ExitCell;
                    if (unknown_cell_index_428a90(cptr->MapCoords) < unknown_cell_index_428a90(cell)) {
                        Cell exit = cptr->Get_Tunnel()->ExitCell;
                        ZoneConnectionClass con;
                        con.From = cptr->MapCoords;
                        con.To = exit;
                        con.unknown_08 = true;
                        con.unknown_0c = 1;
                        ZoneConnections.Add(con);
                    }
                }
            }

        } else {

            int faceindex;
            if (cptr->unknown_47b960()) {
                faceindex = cptr->IsoTileIndex - unknown_tileset_a52c74;
            } else {
                faceindex = cptr->IsoTileIndex - unknown_tileset_a6cb68;
            }

            if (_bridge_subtiles1[faceindex] == (unsigned char)cptr->unknown_11a) {

                bool crossed = false;
                bool passable = true;
                int facing = _facings[faceindex];

                CellClass *bptr;
                for (bptr = &cptr->Adjacent_Cell(facing); Unknown_0054AAB0(bptr->MapCoords) && !crossed; bptr = &bptr->Adjacent_Cell(facing)) {
                    if (bptr->unknown_47b960()) {
                        int index = bptr->IsoTileIndex - unknown_tileset_a52c74;
                        if (index != -1 && _bridge_subtiles2[index] == (unsigned char)bptr->unknown_11a) {
                            crossed = true;
                        }

                    } else if (bptr->unknown_47b980()) {
                        int index = bptr->IsoTileIndex - unknown_tileset_a6cb68;
                        if (index != -1 && _bridge_subtiles2[index] == (unsigned char)bptr->unknown_11a) {
                            crossed = true;
                        }
                    } else if (!bptr->IsUnderBridge) {
                        passable = false;
                    }
                }

                if (crossed) {
                    ZoneConnectionClass con;
                    con.From = cptr->MapCoords;
                    con.To = bptr->Adjacent_Cell((facing - 4) & 7).MapCoords;
                    con.unknown_08 = passable;
                    con.unknown_0c = 0;
                    ZoneConnections.Add(con);
                }
            }
        }

        cptr = Iterate();
    }
}
