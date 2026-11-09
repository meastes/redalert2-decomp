// Behaviour row for native M23 (Scout1187). NOT EXACT.
#include "shared/ebolt.hpp"

// unit.hpp's name for the unit's electric bolt (UnitClass +0x5C4 holds an EBolt:
// Update_All 0x004B2210 / Delete_All 0x004B23B0 clear the same +0x20 owner field).
class UnknownUnit5C4 {
public:
    void unknown_4b25c0();
};

// 0x004B25C0 (pass-6a decompile): the bolt lets go of its owner unit, which forgets
// the bolt (EBolt's inline destructor body without the delete). Reached by
// UnitClass::Save 0x00707DB0 and ~UnitClass 0x006FA7C0 for a unit with a bolt.
void UnknownUnit5C4::unknown_4b25c0()
{
    EBolt *bolt = (EBolt *)this;
    if (bolt->Owner) {
        bolt->Owner->unknown_5c4 = 0;
        bolt->Owner = 0;
    }
}
