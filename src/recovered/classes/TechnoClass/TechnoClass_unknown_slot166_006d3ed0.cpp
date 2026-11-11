// int TechnoClass::unknown_slot166() (0x006D3ED0, slot 166)
// The pips to draw: the type's slot 29 count scaled by ammunition (PipScale
// 1) or passengers (PipScale 3) over the type's maximum, rounded, at least
// one while any remain.
#include "shared/techno.hpp"

int TechnoClass::unknown_slot166()
{
    double amount;
    double maximum;
    switch (Techno_Type_Class()->PipScale) {
    case 1:
        amount = unknown_254;
        maximum = Techno_Type_Class()->Ammo;
        break;
    case 3:
        amount = Cargo.Count;
        maximum = Techno_Type_Class()->Passengers;
        break;
    default:
        return 0;
    }
    int pips = (int)(Techno_Type_Class()->Max_Pips() * (amount / maximum) + 0.5);
    if (pips == 0 && amount > 0.0) {
        pips = 1;
    }
    return pips;
}
