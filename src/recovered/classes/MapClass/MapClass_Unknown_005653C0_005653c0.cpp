// Cell MapClass::Unknown_005653C0(Cell const &cell, Cell const &reference) (0x005653C0)
// The nearer end of the bridge over a cell (TibSun Find_Bridge_Span_End_Cell): a cell not
// under a bridge (+0x140 bit 8) is its own answer; otherwise walks forward (east on an
// east/west bridge, +0x140 bit 11, else south) and back until both walks leave the bridge,
// keeping the last bridge tile cell (0x0047B960 / 0x0047B980) of each whose land type is
// not 3. Ends in the local radar (0x0055A660 by height) qualify; with both, the one
// nearer to reference. The "no cell" value when neither does.
// Walk loop as the accepted sibling 0x00565610 writes it (while (true) with a joint exit test, the
// cells' MapCoords read directly); the end choice is one `||` test (TibSun splits it in two).
#include "shared/map.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/coord_distance.hpp"

Cell MapClass::Unknown_005653C0(Cell const &cell, Cell const &reference)
{
    CellClass *cptr = &Map[cell];

    if (!(cptr->unknown_140 & 0x100)) {
        return cptr->MapCoords;
    }

    int fwd = (cptr->unknown_140 & 0x800) ? 2 : 4;
    int bwd = (fwd - 4) & 7;

    CellClass *fcell = cptr;
    CellClass *bcell = cptr;

    Cell fresult = unknown_cell_a6f238;
    Cell bresult = unknown_cell_a6f238;

    while (true) {
        if (fcell->unknown_140 & 0x100) {
            fcell = &fcell->Adjacent_Cell(fwd);
            if (!(fcell->unknown_140 & 0x100) &&
                (fcell->unknown_47b960() || fcell->unknown_47b980()) && fcell->unknown_ec != 3)
                fresult = fcell->MapCoords;
        }
        if (bcell->unknown_140 & 0x100) {
            bcell = &bcell->Adjacent_Cell(bwd);
            if (!(bcell->unknown_140 & 0x100) &&
                (bcell->unknown_47b960() || bcell->unknown_47b980()) && bcell->unknown_ec != 3)
                bresult = bcell->MapCoords;
        }
        if (!(bcell->unknown_140 & 0x100) && !(fcell->unknown_140 & 0x100)) break;
    }

    Cell result = unknown_cell_a6f238;

    if (Map.Unknown_0055A660(fresult, true)) {
        result = fresult;
    }

    if (Map.Unknown_0055A660(bresult, true)) {
        if (result == unknown_cell_a6f238 || bresult.Distance_To(reference) < fresult.Distance_To(reference)) {
            result = bresult;
        }
    }

    return result;
}
