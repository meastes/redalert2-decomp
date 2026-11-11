// void VeinholeMonsterClass::Shrink() (0x00710FF0)
// Spreads the veins: takes one to four nodes (random) with the lowest keys
// off the heap and grows the vein at each node's cell (0x00712300).
// Body: 646 bytes (ret at 0x00711275); the inventory splits it at 0x00711076.
#include "shared/veinhole.hpp"
#include "shared/map_cell_view.hpp"
#include "shared/random.hpp"
// @ref ?unknown_id_source_a3d290@@3PAVUnknownIDSource@@A=00a3d290
// @ref ??RRandom2Class@@QAEHXZ=00638840
// @ref ?Reduce_Veins_At@VeinholeMonsterClass@@QAEXPAVCellClass@@@Z=00712300

void VeinholeMonsterClass::Shrink()
{
    if (GrowthQueue == 0) {
        return;
    }
    int count = 0;
    int steps = abs(unknown_id_source_a3d290->Random()()) % 4 + 1;
    UnknownVeinNode *node = GrowthQueue->Remove_Min();
    while (count < steps && node != 0) {
        Reduce_Veins_At(&Map[Map[node->Position].MapCoords]);
        count++;
        if (count >= steps) {
            break;
        }
        node = GrowthQueue->Remove_Min();
    }
}
