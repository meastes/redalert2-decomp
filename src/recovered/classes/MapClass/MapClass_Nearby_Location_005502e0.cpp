// Cell MapClass::Unknown_005502E0(Cell const &cell, int speed, int zone, int check, bool checkbridge,
//     Point2D dimensions, bool checkoverlay, bool checkheight, bool checkburrow, bool allowunderbridge,
//     Cell const &nearto, int unknown_scan) (0x005502E0; TibSun MapClass::Nearby_Location)
// Gathers up to 24 cells on successive square rings around `cell` that pass the placement test
// 0x00550CF0 (and optionally the height, burrow and under-bridge checks), stopping at the first ring
// with a cell the tactical view projects back onto itself; returns one at random by frame (visible
// ones first) or, given `nearto`, the closest by Fast_Sqrt distance. Draft by codex2 (99.72); exact
// under the Map unit's TPoint3D coordinate profile (RA2_COORD_TPOINT3D + RA2_COORD_CELL_BASE_INIT,
// first row 0x0042AD30), which makes CoordStruct(Cell, z) TibSun's `: Point3D(x, y, z)` (Scout1005).
#include "shared/map.hpp"
#include "shared/map_cell.hpp"
#include "shared/tactical.hpp"
#include "shared/fast_math.hpp"
#include "shared/crt_memory.hpp"
extern int unknown_frame_a40d2c;
// @ref ?unknown_frame_a40d2c@@3HA=00a40d2c

Cell MapClass::Unknown_005502E0(Cell const &cell, int speed, int zone, int check,
    bool checkbridge, Point2D dimensions, bool checkoverlay, bool checkheight,
    bool checkburrow, bool allowunderbridge, Cell const &nearto, int unknown_scan)
{
    Cell candidates[24];
    int count = 0;
    int xx = cell.X;
    int yy = cell.Y;
    bool found = false;
    if (zone == 0xffff) zone = -1;
    int height = Map[cell].unknown_11b;
    if (checkbridge) {
        if (Map[cell].IsUnderBridge) height += 4;
    }
    int const maxradius = MapSize.Height + MapSize.Width > 32 ? 32 : MapSize.Height + MapSize.Width;
    for (int radius = 0; radius < maxradius; ++radius) {
        int negative_radius = -radius;
        for (int x = negative_radius; x <= radius; ++x) {
            Cell newcell;
            CellClass *cellptr;
            // The existing int formal carries a scan flag observed only through its low byte.
            if ((unsigned char)unknown_scan == 0) {
                newcell = Cell(x + xx, yy - radius);
                cellptr = &Map[newcell];
                if (Unknown_0055A740(cellptr, true) && Unknown_00550CF0(newcell, dimensions.X, dimensions.Y, speed, zone, check, -1, checkbridge, checkoverlay)) {
                    if (!checkheight || abs(height - 4 * cellptr->IsUnderBridge - cellptr->unknown_11b) < 2) {
                        if ((!checkburrow || cellptr->unknown_47c200()) && (allowunderbridge || !cellptr->IsUnderBridge)) {
                            candidates[count++] = newcell;
                            if (checkbridge || newcell == unknown_tactical_83996c->Coord_To_Cell(CoordStruct(newcell, 0))) {
                                found = true;
                            }
                        }
                    }
                }
            }
            if (count == 24) break;
            if ((unsigned char)unknown_scan == 0 || x > negative_radius) {
                int bottom_y = yy + radius;
                newcell = Cell(x + xx, bottom_y);
                cellptr = &Map[newcell];
                if (Unknown_0055A740(cellptr, true) && Unknown_00550CF0(newcell, dimensions.X, dimensions.Y, speed, zone, check, -1, checkbridge, checkoverlay)) {
                    if (!checkheight || abs(height - 4 * cellptr->IsUnderBridge - cellptr->unknown_11b) < 2) {
                        if ((!checkburrow || cellptr->unknown_47c200()) && (allowunderbridge || !cellptr->IsUnderBridge)) {
                            candidates[count++] = newcell;
                            if (checkbridge || newcell == unknown_tactical_83996c->Coord_To_Cell(CoordStruct(newcell, 0))) {
                                found = true;
                            }
                        }
                    }
                }
            }
            if (count == 24) break;
        }
        if (count == 24) break;
        for (int y = negative_radius + 1; y <= radius - 1; ++y) {
            Cell newcell;
            CellClass *cellptr;
            if ((unsigned char)unknown_scan == 0) {
                newcell = Cell(xx - radius, yy + y);
                cellptr = &Map[newcell];
                if (Unknown_0055A740(cellptr, true) && Unknown_00550CF0(newcell, dimensions.X, dimensions.Y, speed, zone, check, -1, checkbridge, checkoverlay)) {
                    if (!checkheight || abs(height - 4 * cellptr->IsUnderBridge - cellptr->unknown_11b) < 2) {
                        if ((!checkburrow || cellptr->unknown_47c200()) && (allowunderbridge || !cellptr->IsUnderBridge)) {
                            candidates[count++] = newcell;
                            if (checkbridge || newcell == unknown_tactical_83996c->Coord_To_Cell(CoordStruct(newcell, 0))) {
                                found = true;
                            }
                        }
                    }
                }
            }
            if (count == 24) break;
            newcell = Cell(xx + radius, yy + y);
            cellptr = &Map[newcell];
            if (Unknown_0055A740(cellptr, true) && Unknown_00550CF0(newcell, dimensions.X, dimensions.Y, speed, zone, check, -1, checkbridge, checkoverlay)) {
                if (!checkheight || abs(height - 4 * cellptr->IsUnderBridge - cellptr->unknown_11b) < 2) {
                    if ((!checkburrow || cellptr->unknown_47c200()) && (allowunderbridge || !cellptr->IsUnderBridge)) {
                        candidates[count++] = newcell;
                        if (checkbridge || newcell == unknown_tactical_83996c->Coord_To_Cell(CoordStruct(newcell, 0))) {
                            found = true;
                        }
                    }
                }
            }
            if (count == 24) break;
        }
        if (count == 24) break;
        if (found) break;
    }
    if (count > 0) {
        Cell visible[24];
        Cell hidden[24];
        int visible_count = 0;
        int hidden_count = 0;
        int index;
        for (index = 0; index < count; ++index) {
            Cell projected = unknown_tactical_83996c->Coord_To_Cell(CoordStruct(candidates[index], 0));
            if (projected == candidates[index]) {
                visible[visible_count++] = candidates[index];
            } else {
                hidden[hidden_count++] = candidates[index];
            }
        }
        if (nearto == unknown_geometry_k) {
            if (visible_count != 0) return visible[unknown_frame_a40d2c % visible_count];
            return hidden[unknown_frame_a40d2c % hidden_count];
        }
        int total = visible_count;
        if (visible_count == 0) total = hidden_count;
        double best_distance = 100000.0;
        Cell best = unknown_geometry_k;
        for (index = 0; index < total; ++index) {
            Cell candidate = visible_count != 0 ? visible[index] : hidden[index];
            int dx = candidate.X - nearto.X;
            int dy = candidate.Y - nearto.Y;
            double distance = Fast_Sqrt((double)(dy * dy + dx * dx));
            if (distance < best_distance) {
                best_distance = distance;
                best = candidate;
            }
        }
        return best;
    }
    return unknown_geometry_k;
}
