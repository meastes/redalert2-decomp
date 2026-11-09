// unsigned char __fastcall Infantry_Action_Allowed_00614b50(int action) (0x00614B50): an
// infantry action is allowed unless planning mode is on (0x00A767A4) and the action is 4
// (InfantryClass 0x00505390 and UnitClass slot 28 0x00703BD0 ask it).
#include "shared/planning.hpp"

unsigned char __fastcall Infantry_Action_Allowed_00614b50(int action)
{
    if (unknown_flag_a767a4 && action == 4) {
        return false;
    }
    return true;
}
