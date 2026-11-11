// VeinholeMonsterClass::Clear_Growth (0x00711C90; Clear_Growth as TiberiumClass names it): drops this monster's growth queue and tables.
#include "shared/veinhole.hpp"
// @ref ??3@YAXPAX@Z=00781d3b

void VeinholeMonsterClass::Clear_Growth()
{
    if (GrowthQueue != 0) {
        GrowthQueue->Clear();
        delete GrowthQueue;
        GrowthQueue = 0;
    }
    if (GrowthNodes != 0) {
        delete[] GrowthNodes;
        GrowthNodes = 0;
    }
    if (GrowthState != 0) {
        delete[] GrowthState;
        GrowthState = 0;
    }
    GrowthCount = 0;
}
