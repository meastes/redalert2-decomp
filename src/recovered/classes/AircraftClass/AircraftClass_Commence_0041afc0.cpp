// bool AircraftClass::Commence() (0x0041AFC0, slot 117)
// Unlocks (+0x5BA), then commences the queued mission.
#include "shared/aircraft.hpp"
// @ref ?Commence@MissionClass@@UAE_NXZ=00594c50

bool AircraftClass::Commence()
{
    unknown_5ba = false;
    return MissionClass::Commence();
}
