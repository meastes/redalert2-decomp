// FootClass's pending-order slots (attack-move, mission 0x1D; C3). NOT EXACT:
// behaviour-written from the exact target's bodies, which the tree carries only as
// seeds (src/recovered/004cda80.cpp, 004cdaa0.cpp, 004cdab0.cpp, 004cdac0.cpp,
// 004cdbd0.cpp; all 31 bytes or less, read in full).
//
// Natively these five FootClass slots had no definition and data/address_aliases.tsv
// bound them to TechnoClass's inline defaults (techno.hpp slots 269-272, 276: do
// nothing, false, false, -1, false). So the pending order FootClass::unknown_slot268
// 0x004CD9C0 records for an attack-move (+0x4B4 = 0x1D, its cell at +0x4B8) was never
// reported: TechnoClass::AI 0x006D9E10 never took slot 276 to scan for enemies on the
// way (slot 278), UnitClass slot 261 0x006FD630 did not keep the order on arrival
// (slot 270), and nothing cleared it (slot 269). An attack-move was a plain move.
#include "shared/foot.hpp"

// 0x004CDA80 (slot 269): clears the pending order.
void FootClass::unknown_slot269()
{
    unknown_4b4 = -1;
    unknown_4b8 = 0;
    unknown_4bc = 0;
    unknown_4c1 = false;
}

// 0x004CDAA0 (slot 270): an order is pending.
bool FootClass::unknown_slot270()
{
    return unknown_4b4 != -1;
}

// 0x004CDAB0 (slot 271): the pending order has been resumed (+0x4C1).
bool FootClass::unknown_slot271()
{
    return unknown_4c1;
}

// 0x004CDAC0 (slot 272): the pending order's kind (-1 for none).
int FootClass::unknown_slot272()
{
    return unknown_4b4;
}

// 0x004CDBD0 (slot 276): the pending order is an attack-move.
bool FootClass::unknown_slot276()
{
    return unknown_4b4 == 0x1D;
}
