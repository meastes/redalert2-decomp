#define SHAPE_FILE_EMPTY_RECT Rect(0, 0, 0, 0)
#include "shared/unknown_globals.hpp"
#include "shared/beacon.hpp"
#include "shared/house.hpp"
#include "shared/cd_timer.hpp"

bool BeaconManagerClass::Is_Flashing_0042efd0()
{
    if (AllocatedCount) {
        bool found = false;
        for (int house = 0; house < 8; house++) {
            int slot = 0;
            do {
                UnknownBeacon *beacon = unknown_00[house][slot];
                if (beacon && (beacon->unknown_flags & 1) && PlayerPtr->Is_Ally(beacon->unknown_house)
                    && Houses[beacon->unknown_house]->Is_Ally(PlayerPtr)
                    && !Houses[beacon->unknown_house]->IsDefeated) {
                    found = true;
                    break;
                }
                slot++;
            } while (slot < 3);
        }
        if (found && Frame % RadarBeaconAnimPeriod < unknown_beacon_phase_84e948 + 1) {
            return true;
        }
    }
    return false;
}
