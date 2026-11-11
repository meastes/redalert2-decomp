// VeinholeMonsterClass::Deinit_Vein_Growth_System (0x007114F0): every monster's Clear_Growth, last first, then the
// shared table (Clear_Global_Data).
#include "shared/veinhole.hpp"
// @ref ?VeinholeMonsters@VeinholeMonsterClass@@2V?$DynamicVectorClass@PAVVeinholeMonsterClass@@@@A=00ad0748
// @ref ?unknown_veinhole_buffer_ad07a0@@3PAEA=00ad07a0
// @ref ??3@YAXPAX@Z=00781d3b

void __cdecl VeinholeMonsterClass::Deinit_Vein_Growth_System()
{
    for (int index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        VeinholeMonsters[index]->Clear_Growth();
    }
    Clear_Global_Data();
}
