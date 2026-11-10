// AStarClass::Ban_Subzone_Edge() (0x0042A7D0): records a subzone
// pair in a layer's list.
#include "shared/path_estimator.hpp"

void AStarClass::Ban_Subzone_Edge(unsigned first, unsigned second, int layer)
{
    if (first == second) {
        return;
    }
    if (second < first) {
        unsigned swap = second;
        second = first;
        first = swap;
    }
    HierBannedEdges[layer].Add((first << 16) | second);
}
