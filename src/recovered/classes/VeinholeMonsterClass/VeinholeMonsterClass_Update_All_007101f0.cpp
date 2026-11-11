// void VeinholeMonsterClass::Update_All() (0x007101F0)
// Runs every monster's AI, last first, then deletes the dead ones.
#include "shared/veinhole.hpp"

void __cdecl VeinholeMonsterClass::Update_All()
{
    for (int index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        VeinholeMonsters[index]->AI();
    }
    Remove_Dead();
}
