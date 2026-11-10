// int CellClass::unknown_47c040() (0x0047C040): tells the object found for
// the cell (Unknown_007101B0; tube or bridge hypothesis) about the cell, or
// else runs Redraw_Veins on the map's cell; answers 0.
#include "shared/cell.hpp"
#include "shared/map_view.hpp"
// @ref ?Unknown_007101B0@@YIPAVUnknownCellLookup@@ABVCell@@@Z=007101b0
// @ref ?Unknown_00712300@UnknownCellLookup@@QAEXPAVCellClass@@@Z=00712300
// @ref ?Redraw_Veins@CellClass@@QAEXXZ=0047a7a0

int CellClass::unknown_47c040()
{
    UnknownCellLookup *lookup = Unknown_007101B0(MapCoords);
    if (lookup != 0) {
        lookup->Unknown_00712300(this);
        return 0;
    }
    Map[MapCoords].Redraw_Veins();
    return 0;
}
