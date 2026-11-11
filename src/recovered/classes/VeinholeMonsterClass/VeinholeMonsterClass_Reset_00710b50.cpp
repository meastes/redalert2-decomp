// void VeinholeMonsterClass::Reset() (0x00710B50)
// Deletes every monster and empties VeinholeMonsters.
#include "shared/veinhole.hpp"

void __cdecl VeinholeMonsterClass::Reset()
{
    for (int index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        delete VeinholeMonsters[index];
    }
    VeinholeMonsters.Clear();
}
