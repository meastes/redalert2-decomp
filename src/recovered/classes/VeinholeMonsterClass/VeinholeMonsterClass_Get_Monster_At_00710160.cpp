// VeinholeMonsterClass *VeinholeMonsterClass::Get_Monster_At(Cell const &cell) (0x00710160)
// The last monster in VeinholeMonsters whose cell is cell, or null.
#include "shared/veinhole.hpp"
#include "shared/house.hpp"

VeinholeMonsterClass *__fastcall VeinholeMonsterClass::Get_Monster_At(::Cell const &cell)
{
    for (int index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        if (VeinholeMonsters[index]->CellID == cell) {
            return VeinholeMonsters[index];
        }
    }
    return 0;
}
