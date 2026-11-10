// Select a bridge span end or side cell in the requested subzone, original00565610.
#include "shared/map.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/map_adjacent.hpp"
#include "shared/cell_subzone.hpp"

Cell MapClass::Find_Bridge_End_Cell_For_Subzone(Cell const &cell, int layer, unsigned int subzone)
{
    CellClass *cellptr = &Map[cell];
    if (!cellptr->IsUnderBridge) return cellptr->MapCoords;
    int facing = cellptr->IsBridgeEastWest ? 2 : 4;
    int opposite = (facing - 4) & 7;
    CellClass *forward = cellptr;
    CellClass *back = cellptr;
    // Four-byte point-base view; the original local nominal type remains unknown.
    TPoint2D<short> front_end = unknown_cell_a6f238;
    Cell back_end = unknown_cell_a6f238;
    while (true) {
        if (forward->IsUnderBridge) {
            forward = &forward->Adjacent_Cell(facing);
            if (!forward->IsUnderBridge &&
                (forward->unknown_47b960() || forward->unknown_47b980()) && forward->unknown_ec != 3)
                front_end = forward->MapCoords;
        }
        if (back->IsUnderBridge) {
            back = &back->Adjacent_Cell(opposite);
            if (!back->IsUnderBridge &&
                (back->unknown_47b960() || back->unknown_47b980()) && back->unknown_ec != 3)
                back_end = back->MapCoords;
        }
        if (!back->IsUnderBridge && !forward->IsUnderBridge) break;
    }
    Cell candidates[6];
    candidates[0] = front_end;
    candidates[1] = Adjacent_Cell(front_end, (facing + 2) & 7);
    candidates[2] = Adjacent_Cell(front_end, (facing - 2) & 7);
    candidates[3] = back_end;
    candidates[4] = Adjacent_Cell(back_end, (facing + 2) & 7);
    candidates[5] = Adjacent_Cell(back_end, (facing - 2) & 7);
    for (int index = 0; index < 6; ++index) {
        Cell candidate = candidates[index];
        if (Map.Unknown_0055A660(candidate, true)) {
            int fine_index = Get_Cell_Zone_Index(candidate);
            CellSubzoneStruct *record = (CellSubzoneStruct *)CellSubzones + fine_index;
            if (record->SubzoneID[layer] == subzone)
                return candidate;
        }
    }
    return unknown_cell_a6f238;
}
