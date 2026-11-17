#define SHAPE_FILE_EMPTY_RECT Rect(0, 0, 0, 0)
#include "shared/unknown_globals.hpp"
#include "shared/beacon.hpp"
#include "shared/house.hpp"
#include "shared/cd_timer.hpp"

void BeaconManagerClass::Draw_On_Radar_0042eed0(Surface *surface, Rect rect)
{
    if (AllocatedCount && Frame % RadarBeaconAnimPeriod < unknown_beacon_phase_84e948 + 1) {
        for (int house = 0; house < 8; house++) {
            for (int slot = 0; slot < 3; slot++) {
                UnknownBeacon *beacon = unknown_00[house][slot];
                if (beacon && (beacon->unknown_flags & 1) && PlayerPtr->Is_Ally(beacon->unknown_house)
                    && Houses[beacon->unknown_house]->Is_Ally(PlayerPtr)
                    && !Houses[beacon->unknown_house]->IsDefeated) {
                    beacon->Unknown_0042DE20(surface, rect, false);
                }
            }
        }
    }
}
