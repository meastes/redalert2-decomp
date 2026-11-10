// bool CellClass::Is_Near_Tunnel_NW() (0x00479CF0): Westwood's Is_Near_Tunnel_NW. This cell holds
// a tunnel, or one of the next two cells to the north or to the west holds a tunnel mouth (a tunnel
// cell whose next cell on that line has none).
#include "shared/cell.hpp"
#include "shared/map_view.hpp"

bool CellClass::Is_Near_Tunnel_NW()
{
    if (Has_Tunnel()) {
        return true;
    }

    CellClass *north1 = &Adjacent_Cell(0);
    if (north1 != 0) {
        CellClass *north2 = &north1->Adjacent_Cell(0);
        if (north2 != 0) {
            if (north1->Has_Tunnel() && !north2->Has_Tunnel()) {
                return true;
            }
            CellClass *north3 = &north2->Adjacent_Cell(0);
            if (north3 != 0) {
                if (north2->Has_Tunnel() && !north3->Has_Tunnel()) {
                    return true;
                }
            }
        }
    }

    CellClass *west1 = &Adjacent_Cell(6);
    if (west1 != 0) {
        CellClass *west2 = &west1->Adjacent_Cell(6);
        if (west2 != 0) {
            if (west1->Has_Tunnel() && !west2->Has_Tunnel()) {
                return true;
            }
            CellClass *west3 = &west2->Adjacent_Cell(6);
            if (west3 != 0) {
                if (west2->Has_Tunnel() && !west3->Has_Tunnel()) {
                    return true;
                }
            }
        }
    }

    return false;
}
