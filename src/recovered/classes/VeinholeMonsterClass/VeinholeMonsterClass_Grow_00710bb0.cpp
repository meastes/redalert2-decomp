// void VeinholeMonsterClass::Grow() (0x00710BB0)
// Grows the vein network while the monster has room for nodes and the
// scenario allows it (+0x222B): takes one to five nodes off the heap; each
// node's cell is grown (0x0047AD00) and claimed, and its four straight
// neighbours on the map, within one level and accepted by 0x0047A670, not
// yet claimed, become new nodes keyed by the frame / 50 plus a random 1..50.
#include "shared/veinhole.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/random.hpp"
#include "shared/adjacent_cell.hpp"
// @ref ?unknown_id_source_a3d290@@3PAVUnknownIDSource@@A=00a3d290
// @ref ??RRandom2Class@@QAEHXZ=00638840
// @ref ?unknown_cells_428ac0@@YIHXZ=00428ac0
// @ref ?unknown_veinhole_buffer_ad07a0@@3PAEA=00ad07a0
// @ref ?unknown_cell_index_428a90@@YIHABVCell@@@Z=00428a90
// @ref ?Sift_Down@UnknownVeinHeap@@QAEXH@Z=0058f090

void VeinholeMonsterClass::Grow()
{
    if (GrowthQueue == 0) {
        return;
    }
    if (GrowthCount > Rules_Vein_Node_Count() - 40) {
        return;
    }
    if (VeinCount > Rules_Vein_Node_Count() - 100) {
        return;
    }
    if (!unknown_id_source_a3d290->unknown_222b) {
        return;
    }
    int count = 0;
    Random2Class &random = unknown_id_source_a3d290->Random();
    int steps = abs(random()) % 5 + 1;
    UnknownVeinNode *node = GrowthQueue->Remove_Min();
    while (count < steps && node != 0) {
        CellClass *cell = &Map[node->Position];
        if (cell->OverlayData < 0x30) {
            cell->Place_Veins();
            VeinCount++;
        }
        if (cell->OverlayIndex == 0x7e) {
            int index = unknown_cell_index_428a90(node->Position);
            if (index >= 0 && index < unknown_cells_428ac0()) {
                GrowthState[index] = 1;
            }
        }
        if (cell->OverlayData >= 0x30) {
            for (int facing = 0; facing < 8; facing += 2) {
                ::Cell spot = node->Position + AdjacentCell[facing & 7];
                if (!Map.Unknown_0055A660(spot, true)) {
                    continue;
                }
                CellClass *adjacent = &Map[spot];
                if (abs(adjacent->unknown_11b - cell->unknown_11b) >= 2) {
                    continue;
                }
                int index = unknown_cell_index_428a90(spot);
                if (index < 0 || index >= unknown_cells_428ac0()) {
                    continue;
                }
                if (adjacent->Can_Place_Veins() && unknown_veinhole_buffer_ad07a0[index] == 0 && GrowthCount < Rules_Vein_Node_Count()) {
                    GrowthNodes[GrowthCount].Position = spot;
                    GrowthNodes[GrowthCount].Key = Frame / 50 + abs(unknown_id_source_a3d290->Random()() % 50) + 1;
                    unknown_veinhole_buffer_ad07a0[index] = 1;
                    GrowthQueue->Insert(&GrowthNodes[GrowthCount++]);
                }
                GrowthState[index] = 1;
            }
        }
        count++;
        if (count >= steps) {
            break;
        }
        node = GrowthQueue->Remove_Min();
    }
}
