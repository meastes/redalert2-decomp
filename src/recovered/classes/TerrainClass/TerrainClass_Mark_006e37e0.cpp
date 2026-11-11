// bool TerrainClass::Mark(int arg1) (0x006E37E0)
// After the base handling succeeds, takes the object off its cell (mode 0)
// or puts it on the cell (modes 1 and 3).
#include "shared/terrain.hpp"
// @ref ?Mark@ObjectClass@@UAE_NH@Z=005d4160
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Place_Down@MapClass@@QAEXABVCell@@PAVObjectClass@@@Z=0054ab70
// @ref ?Pick_Up@MapClass@@QAEXABVCell@@PAVObjectClass@@@Z=0054af70

bool TerrainClass::Mark(int arg1)
{
    if (ObjectClass::Mark(arg1)) {
        Cell cell = unknown_slot104();
        switch (arg1) {
        case 0:
            Map.Pick_Up(cell, this);
            break;
        case 1:
        case 3:
            Map.Place_Down(cell, this);
            return true;
        }
        return true;
    }
    return false;
}
