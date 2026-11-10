// AStarClass::Reset() (0x00429A90): reallocates and
// clears the three layers' arrays.
#include "shared/path_estimator.hpp"
#include "shared/map_view.hpp"

void AStarClass::Reset()
{
    for (int layer = 0; layer < 3; layer++) {
        if (HierOnPath[layer]) {
            delete HierOnPath[layer];
        }
        int *array = new int[Map.unknown_70[layer]];
        HierOnPath[layer] = array;
        for (int entry = Map.unknown_70[layer] - 1; entry >= 0; entry--) {
            array[entry] = 0;
        }
        if (HierOpened[layer]) {
            delete HierOpened[layer];
        }
        array = new int[Map.unknown_70[layer]];
        HierOpened[layer] = array;
        for (entry = Map.unknown_70[layer] - 1; entry >= 0; entry--) {
            array[entry] = 0;
        }
        if (HierCosts[layer]) {
            delete HierCosts[layer];
        }
        array = new int[Map.unknown_70[layer]];
        HierCosts[layer] = array;
        for (entry = Map.unknown_70[layer] - 1; entry >= 0; entry--) {
            array[entry] = 0;
        }
    }
}
