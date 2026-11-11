#include "shared/veinhole.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/map_view.hpp"
// @ref ?Build_Shrinking_Queue@VeinholeMonsterClass@@QAEXXZ=00711aa0

void VeinholeMonsterClass::Destroy_Monster()
{
    CellClass &cptr = Map[CellID];
    cptr.OverlayIndex = 0x7e;
    cptr.OverlayData = 0;
    cptr.Recalc_Attributes(-1);
    for (int dir = 0; dir < 8; dir++) {
        CellClass &adj = cptr.Adjacent_Cell(dir);
        if (dir % 2 != 0) {
            adj.OverlayIndex = -1;
            adj.OverlayData = 0;
        } else {
            adj.OverlayIndex = 0x7e;
            adj.OverlayData = 0;
        }
    }
    for (int y = -2; y <= 2; y++) {
        for (int x = -2; x <= 2; x++) {
            CellClass &cptr2 = Map[::Cell(x, y) + CellID];
            cptr2.Place_Veins();
            cptr2.Recalc_Attributes(-1);
        }
    }
    Build_Shrinking_Queue();
    Map.Update_Cell_Zone(CellID);
    Map.Unknown_00566340(CellID);
    IsDead = true;
}
