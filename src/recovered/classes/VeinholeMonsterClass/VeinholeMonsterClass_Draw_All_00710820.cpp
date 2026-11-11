// void VeinholeMonsterClass::Draw_All() (0x00710820)
// Calls Draw_It on every monster, last first.
#include "shared/veinhole.hpp"
// @ref ?Draw_It@VeinholeMonsterClass@@QAEXXZ=00710880

void __cdecl VeinholeMonsterClass::Draw_All()
{
    for (int index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        VeinholeMonsters[index]->Draw_It();
    }
}
