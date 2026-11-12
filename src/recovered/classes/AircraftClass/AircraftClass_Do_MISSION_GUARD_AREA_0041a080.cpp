// int AircraftClass::Do_MISSION_GUARD_AREA() (0x0041A080, slot 130)
// At the type's flight level: slot 261 (0, 1) unless in a team, 1.
// Otherwise slot 261 when out of ammunition, slot 163 holds and there is
// no radio contact; with a target, queue mission 1; else FootClass's.
#include "shared/aircraft.hpp"
// @ref ?Do_MISSION_GUARD_AREA@FootClass@@UAEHXZ=004c5900
// @ref ?HasAnyLink@RadioClass@@QBE_NXZ=00636f10

int AircraftClass::Do_MISSION_GUARD_AREA()
{
    if (Get_Height_AGL() == Type->Flight_Level()) {
        if (Team == 0) {
            unknown_slot261(0, 1);
        }
        return 1;
    }
    if (unknown_254 == 0 && unknown_slot163() && !HasAnyLink()) {
        unknown_slot261(0, 1);
    }
    if (unknown_228 != 0) {
        QueueMission(1, false);
        return 1;
    }
    return FootClass::Do_MISSION_GUARD_AREA();
}
