// VeinholeMonsterClass *VeinholeMonsterClass::Get_Vein_Owner_At(Cell const &cell) (0x007101B0)
// The last monster in VeinholeMonsters whose per-cell table (+0xB0) has a byte set at the cell, or null.
#include "shared/veinhole.hpp"
// @ref ?unknown_cell_index_428a90@@YIHABVCell@@@Z=00428a90

VeinholeMonsterClass *__fastcall VeinholeMonsterClass::Get_Vein_Owner_At(::Cell const &cell)
{
    for (int index = VeinholeMonsters.Count() - 1; index >= 0; index--) {
        if (VeinholeMonsters[index]->GrowthState[unknown_cell_index_428a90(cell)] != 0) {
            return VeinholeMonsters[index];
        }
    }
    return 0;
}
