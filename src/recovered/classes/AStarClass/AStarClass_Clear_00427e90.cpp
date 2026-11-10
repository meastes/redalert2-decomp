// AStarClass::Clear() (0x00427E90): starts a new search.
#include "shared/path_estimator.hpp"
#include "shared/map_view.hpp"
// Shared map-grid dimension; this pathfinding routine needs no MapRegionClass layout.
extern int unknown_map_cell_side_84e804;

void AStarClass::Clear()
{
    RegularNodes->count = 0;
    RegularOpenNodes->count = 0;
    RegularQueue->Clear();
    HierQueue->Clear();
    if (++UniqueID == 0) {
        for (int index = unknown_map_cell_side_84e804 * unknown_map_cell_side_84e804 - 1; index >= 0; index--) {
            RegularVisited[index] = 0;
            RegularBridgeVisited[index] = 0;
        }
        for (int layer = 0; layer < 3; layer++) {
            int size = Map.unknown_70[layer];
            int *first = HierOnPath[layer];
            int *second = HierOpened[layer];
            int *third = HierCosts[layer];
            for (int entry = size - 1; entry >= 0; entry--) {
                first[entry] = 0;
                second[entry] = 0;
                third[entry] = 0;
            }
        }
        UniqueID++;
    }
}
