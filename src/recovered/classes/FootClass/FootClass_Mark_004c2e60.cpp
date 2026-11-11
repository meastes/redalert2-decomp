// bool FootClass::Mark(int arg1) (0x004C2E60)
// Mode 2 always succeeds; otherwise TechnoClass's slot 71, and in layer 2
// the map is told about the object at its cell (0x0054AF70 for mode 0,
// 0x0054AB70 for modes 1 and 3).
#include "shared/foot.hpp"
#include "shared/map_view.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Place_Down@MapClass@@QAEXABVCell@@PAVObjectClass@@@Z=0054ab70
// @ref ?Pick_Up@MapClass@@QAEXABVCell@@PAVObjectClass@@@Z=0054af70
// @ref ?Mark@TechnoClass@@UAE_NH@Z=006c2550

bool FootClass::Mark(int arg1)
{
    if (arg1 == 2) {
        return true;
    }
    if (TechnoClass::Mark(arg1)) {
        if (In_Which_Layer() == 2) {
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
        }
        return true;
    }
    return false;
}
