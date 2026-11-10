// AStarClass::Ban_Neighborhood_Subzone_Edges() (0x0042A840): records the subzone
// pairs around a subzone's step on a layer's +0xBC path.
#include "shared/path_estimator.hpp"
#include "shared/map_view.hpp"
#include "shared/subzone_tracking.hpp"
// @ref ?Add@?$DynamicVectorClass@I@@QAE_NABI@Z=0042b0f0

void AStarClass::Ban_Neighborhood_Subzone_Edges(unsigned subzone, int layer)
{
    int count = HierSubzonePathCount[layer];
    if (count <= 1) {
        IsHSEnabled = false;
        return;
    }
    int found = -1;
    for (int index = 0; index < count; index++) {
        if (HierSubzonePath[layer][index] == subzone) {
            found = index;
            break;
        }
    }
    if (found == -1) {
        IsHSEnabled = false;
    } else {
        unsigned short second;
        unsigned short first;
        if (found == count - 1) {
            second = HierSubzonePath[layer][found];
            first = HierSubzonePath[layer][found - 1];
        } else {
            second = HierSubzonePath[layer][found + 1];
            first = HierSubzonePath[layer][found];
        }
        Ban_Subzone_Edge(second, first, layer);
        DynamicVectorClass<SubzoneConnectionStruct> &to = Map.SubzoneTracking[layer][second].Connections;
        DynamicVectorClass<SubzoneConnectionStruct> &from = Map.SubzoneTracking[layer][first].Connections;
        for (int other = to.Count() - 1; other >= 0; other--) {
            unsigned short next = to[other].unknown_00;
            if (next != first) {
                for (int index = from.Count() - 1; index >= 0; index--) {
                    if (from[index].unknown_00 == next) {
                        Ban_Subzone_Edge(first, next, layer);
                    }
                }
            }
        }
    }
}
