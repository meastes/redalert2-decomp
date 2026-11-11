// void VeinholeMonsterClass::Remove_Dead() (0x00711E20)
// Deletes every placed monster (IsDead) whose node heap is empty, last
// first (expanded into Update_All 0x007101F0).
#include "shared/veinhole.hpp"

void __cdecl VeinholeMonsterClass::Remove_Dead()
{
    for (int index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        UnknownVeinHeap *heap = VeinholeMonsters[index]->GrowthQueue;
        if (VeinholeMonsters[index]->IsDead && heap->Count == 0) {
            delete VeinholeMonsters[index];
        }
    }
}
