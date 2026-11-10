// AStarClass::Is_Visited() (0x00427F70): whether a cell index is
// marked in the current generation.
#include "shared/path_estimator.hpp"

bool AStarClass::Is_Visited(int unused, bool first, int index)
{
    if (first) {
        if (RegularVisited[index] == UniqueID) return true;
        return false;
    }
    if (RegularBridgeVisited[index] == UniqueID) return true;
    return false;
}
