// int AircraftClass::Do_MISSION_PARADROP_APPROACH() (0x00415190, slot 146)
// Without a target: clear it and queue mission 4. With one but no +0x494:
// slot 260 on it. Within the rules distance (+0x468): queue mission 27 and
// use up one of +0x5BB. Always 3.
#include "shared/aircraft.hpp"
// @ref ?Rules@@3PAVRulesClass@@A=00839848
// @ref ?Planar_Distance@ObjectClass@@QBEHPBVAbstractClass@@@Z=005d4ce0

int AircraftClass::Do_MISSION_PARADROP_APPROACH()
{
    if (unknown_228 == 0) {
        Assign_Destination(0, true);
        unknown_slot116(4, false);
        return 3;
    }
    if (unknown_494 == 0) {
        Assign_Destination(unknown_228, true);
        return 3;
    }
    if (Planar_Distance(unknown_228) <= Rules->unknown_468) {
        unknown_slot116(27, false);
        unknown_5bb--;
    }
    return 3;
}
