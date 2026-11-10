// bool CellClass::Is_Near_Tunnel_ES() (0x00479F70): TibSun name and shape. This cell, the next
// two to the south-east, or the cells south and east of the first of them hold a tunnel
// (Has_Tunnel 0x00479CC0, expanded at every site).
#include "shared/cell.hpp"
#include "shared/map_view.hpp"

bool CellClass::Is_Near_Tunnel_ES()
{
    if (Has_Tunnel()) {
        return true;
    }

    CellClass *south_east = &Adjacent_Cell(3);
    if (south_east != 0) {
        CellClass *south_east2 = &south_east->Adjacent_Cell(3);
        if (south_east2 != 0) {
            if (south_east->Has_Tunnel() || south_east2->Has_Tunnel()) {
                return true;
            }

            CellClass *south = &south_east->Adjacent_Cell(4);
            CellClass *east = &south_east->Adjacent_Cell(2);
            if (south != 0 && east != 0) {
                if (south->Has_Tunnel() || east->Has_Tunnel()) {
                    return true;
                }
            }
        }
    }

    return false;
}
