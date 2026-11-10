// void MapClass::Reset_All_Subzones() (0x00563D40)
// Rebuilds the subzone tracking: each of the three vectors, from the last,
// is cleared and refilled by 0x00563D80; then the path estimator is reset.
#include "shared/map.hpp"
#include "shared/path_estimator.hpp"
// @ref ?Reset_Subzone@MapClass@@QAEXH@Z=00563d80
// @ref ?Search@@3UUnknownPathEstimator@@A=008315c8
// @ref ?Reset@AStarClass@@QAEXXZ=00429a90

void MapClass::Reset_All_Subzones()
{
    for (int index = 2; index >= 0; index--) {
        SubzoneTracking[index].Clear();
        Reset_Subzone(index);
    }
    Search.Reset();
}
