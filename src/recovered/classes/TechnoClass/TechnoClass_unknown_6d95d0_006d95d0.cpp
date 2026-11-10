// void TechnoClass::unknown_6d95d0(UnknownCellClass *cell) (0x006D95D0)
// Removes the owner's +0x400 from the cell (0x00476B20) and clears it.
#include "shared/techno.hpp"
// @ref ?Adjust_Threat@UnknownCellClass@@QAEXHH@Z=00476b20

void TechnoClass::unknown_6d95d0(UnknownCellClass *cell)
{
    if (cell != 0) {
        cell->Adjust_Threat(Owner(), -unknown_400);
        unknown_400 = 0;
    }
}
