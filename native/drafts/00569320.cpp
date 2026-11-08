// NON-EXACT DRAFT 0x569320 FUN_00569320
// entry 0x569320; exact-match score 99.49 (misses_index best); plan "draft as-is"
// origin lane: Scout889 (build/class-work/Scout889/w/keep/d_9949_head.cpp)
// Recovered source text by our lanes/workers; never exact-match credit. Tracked by
// Scout1061 (native/drafts.json). Not original bytes.
// MapClass::Unknown_00569320: whether a bridge repair hut's bridge is intact (hypothesis).
#include "shared/map.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/map_isotile.hpp"
#include "shared/map_adjacent.hpp"
// @ref ?unknown_bridge_entry_x_7e0514@@3PAFA=007e0514
// @ref ?unknown_bridge_entry_y_7e0534@@3PAFA=007e0534
// @ref ?unknown_bridge_facings_7e0554@@3PAHA=007e0554
extern short unknown_bridge_entry_x_7e0514[];
extern short unknown_bridge_entry_y_7e0534[];
extern int unknown_bridge_facings_7e0554[];


static inline bool Is_Hut1(int overlay)
{
    return overlay == 0x64 || overlay == 0x65;
}

static inline bool Is_Hut2(int overlay)
{
    return overlay == 0xE7 || overlay == 0xE8;
}

static inline bool Is_Span_EW(int overlay, int base)
{
    return (overlay >= base && overlay <= base + 8) || (overlay >= base + 0x12 && overlay <= base + 0x15) || overlay == base + 0x1A;
}

bool MapClass::Unknown_00569320(Cell const &cell)
{
    bool found = false;
    bool second_low_bridge = false;
    CellClass *cellptr = 0;
    int offset = -1;
    for (int y = -2; y < 3; ++y) {
        for (int x = -2; x < 3; ++x) {
            int tile = Map[Cell(Cell(x, y) + cell)].IsoTileIndex;
            if (tile >= unknown_tileset_a6cb68 && tile < unknown_tileset_a6cb68 + 16) {
                found = true;
                cellptr = &Map[Cell(Cell(x, y) + cell)];
                offset = tile - unknown_tileset_a6cb68;
            } else if (tile >= unknown_tileset_a52c74 && tile < unknown_tileset_a52c74 + 16) {
                found = true;
                cellptr = &Map[Cell(Cell(x, y) + cell)];
                offset = tile - unknown_tileset_a52c74;
            } else {
                if (Unknown_00562930(Cell(x, y) + cell)) {
                    found = false;
                    second_low_bridge = false;
                    cellptr = &Map[Cell(Cell(x, y) + cell)];
                } else if (Unknown_00562980(Cell(x, y) + cell)) {
                    found = false;
                    second_low_bridge = true;
                    cellptr = &Map[Cell(Cell(x, y) + cell)];
                }
            }
        }
    }
    switch (found) {
    case false:
        if (cellptr != 0) {
        if (second_low_bridge) {
            int overlay = cellptr->OverlayIndex;
            if (Is_Span_EW(overlay, 0xCD)) {
                for (Cell walk2 = cellptr->MapCoords; ; walk2 -= Cell(1, 0)) {
                    if (!Unknown_00562980(walk2)) break;
                    int overlay = Map[walk2].OverlayIndex;
                    if (overlay == 0xE7 || overlay == 0xE8) return true;
                }
                for (Cell scan2 = cellptr->MapCoords; ; scan2.X++) {
                    if (!Unknown_00562980(scan2)) break;
                    int overlay = Map[scan2].OverlayIndex;
                    if (overlay == 0xE7 || overlay == 0xE8) return true;
                }
            } else {
                for (Cell scan = cellptr->MapCoords; ; scan.Y++) {
                    if (!Unknown_00562980(scan)) break;
                    int overlay = Map[scan].OverlayIndex;
                    if (overlay == 0xE7 || overlay == 0xE8) return true;
                }
                for (Cell walk = cellptr->MapCoords; ; walk -= Cell(0, 1)) {
                    if (!Unknown_00562980(walk)) break;
                    int overlay = Map[walk].OverlayIndex;
                    if (overlay == 0xE7 || overlay == 0xE8) return true;
                }
            }
        } else {
            int overlay = cellptr->OverlayIndex;
            if (Is_Span_EW(overlay, 0x4A)) {
                for (Cell walk2 = cellptr->MapCoords; ; walk2 -= Cell(1, 0)) {
                    if (!Unknown_00562930(walk2)) break;
                    int overlay = Map[walk2].OverlayIndex;
                    if (overlay == 0x64 || overlay == 0x65) return true;
                }
                for (Cell scan2 = cellptr->MapCoords; ; scan2.X++) {
                    if (!Unknown_00562930(scan2)) break;
                    int overlay = Map[scan2].OverlayIndex;
                    if (overlay == 0x64 || overlay == 0x65) return true;
                }
            } else {
                for (Cell scan = cellptr->MapCoords; ; scan.Y++) {
                    if (!Unknown_00562930(scan)) break;
                    int overlay = Map[scan].OverlayIndex;
                    if (overlay == 0x64 || overlay == 0x65) return true;
                }
                for (Cell walk = cellptr->MapCoords; ; walk -= Cell(0, 1)) {
                    if (!Unknown_00562930(walk)) break;
                    int overlay = Map[walk].OverlayIndex;
                    if (overlay == 0x64 || overlay == 0x65) return true;
                }
            }
        }
        }
        break;
    default:
        IsometricTileTypeClass *type = IsoTileTypes[cellptr->IsoTileIndex];
        int subtile = (unsigned char)cellptr->unknown_11a;
        Cell topleft = cellptr->MapCoords - Cell(subtile % type->unknown_2b4, subtile / type->unknown_2b4);
        Cell search = topleft + Cell(unknown_bridge_entry_x_7e0514[offset], unknown_bridge_entry_y_7e0534[offset]);
        int dir = unknown_bridge_facings_7e0554[offset];
        if (dir != -1) {
            Cell entry = search;
            while (true) {
                int index = Unknown_005500D0(search, 3, 0);
                if (index < 0) break;
                if (!ZoneConnections[index].unknown_08) return true;
                ZoneConnectionClass *con = &ZoneConnections[index];
Cell next;
if (con->From == entry) {
entry = con->To;
                    next = Adjacent_Cell(entry, dir);
} else {
 if (con->To != entry) return true;
entry = con->From;
                    next = Adjacent_Cell(entry, dir);
}
search = next;

            }
        }
        break;
    }
    return false;
}
