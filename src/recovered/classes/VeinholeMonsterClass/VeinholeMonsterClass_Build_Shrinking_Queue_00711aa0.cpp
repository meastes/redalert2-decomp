#include "shared/veinhole.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/map_view.hpp"
#include "shared/anim.hpp"
// @ref ?unknown_cells_428ac0@@YIHXZ=00428ac0
// @ref ?unknown_cell_index_428a90@@YIHABVCell@@@Z=00428a90

void VeinholeMonsterClass::Build_Shrinking_Queue()
{
    GrowthCount = 0;
    GrowthQueue->Clear();
    VeinCount = 0;

    Map.Reset_Iterator();
    CellClass *iter = Map.Iterate();
    while (iter != 0) {
        if (iter->OverlayIndex == 0x7e && iter->OverlayData >= 0x30) {
            int index = unknown_cell_index_428a90(iter->MapCoords);
            if (index >= 0 && index < unknown_cells_428ac0() && GrowthState[index]) {
                if (GrowthCount < Rules_Vein_Node_Count()) {
                    GrowthNodes[GrowthCount].Position = iter->MapCoords;
                    Point2D diff = Point2D(iter->MapCoords.X, iter->MapCoords.Y) - Point2D(CellID.X, CellID.Y);
                    GrowthNodes[GrowthCount].Key = 1000 - Cell_Distance(diff.X, diff.Y);
                    GrowthQueue->Insert(&GrowthNodes[GrowthCount++]);
                    VeinCount++;
                }
            }
        }
        iter = Map.Iterate();
    }
}
