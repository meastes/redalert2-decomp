// void MissionClass::Override_Mission(int mission, AbstractClass *target, AbstractClass *destination) (0x00594D20, slot 119)
// Overrides the mission, suspending the current (or queued) one.
#include "shared/mission.hpp"

void MissionClass::Override_Mission(int mission, AbstractClass *target, AbstractClass *destination)
{
    if (Mission == 28 && mission == 5) {
        return;
    }
    if (Mission == 19) {
        return;
    }
    if (MissionQueue != -1) {
        SuspendedMission = MissionQueue;
    } else {
        SuspendedMission = Mission;
    }
    Mission = mission;
    unknown_a4 = false;
}
