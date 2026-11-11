// void MapClass::Unknown_0055A300() (0x0055A300)
// Refreshes the shroud drawing of every cell in two walks (0x0055A490): cells
// flagged +0x140 bit 0x20 lose their +0x12C bits 0x18 and +0x140 bits 0x23 and
// are redrawn; then each cell whose layer 0 shroud frame (0x006A8D40) changed
// takes it and is redrawn. A redrawn cell that is fully shrouded (frame -2)
// reveals its nearby technos to the player (0x00568520).
#include "shared/map.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/map_adjacent.hpp"

void MapClass::Unknown_0055A300()
{
    Reset_Iterator();
    CellClass *cellptr = Iterate();
    while (cellptr != 0) {
        if (cellptr->IsToShroud) {
            cellptr->IsMapped = false; cellptr->IsVisible = false;
            cellptr->IsFogVisible = false; cellptr->IsFogMapped = false; cellptr->IsToShroud = false;
            cellptr->unknown_138 = true;
            unknown_tactical_83996c->Flag_Cell(cellptr);
            if (cellptr->unknown_120 == -2) {
                Map.Unknown_00568520(cellptr, unknown_player_a35db4, true);
            }
        }
        cellptr = Iterate();
    }

    Reset_Iterator();
    cellptr = Iterate();
    while (cellptr != 0) {
        Cell cell = Coord_Cell(cellptr->Center_Coord());
        signed char frame = unknown_tactical_83996c->Cell_Shadow(cell, 0);
        if (frame != cellptr->unknown_120) {
            cellptr->unknown_120 = frame;
            cellptr->unknown_138 = true;
            unknown_tactical_83996c->Flag_Cell(cellptr);
            if (cellptr->unknown_120 == -2) {
                Map.Unknown_00568520(cellptr, unknown_player_a35db4, true);
            }
        }
        cellptr = Iterate();
    }
}
