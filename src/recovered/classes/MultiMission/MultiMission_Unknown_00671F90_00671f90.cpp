// The first disk id of a multiplayer mission (0x00671F90). Draft: Scout1076.
#define VECTOR_IMPLEMENT_ALL
#include "shared/multi_mission.hpp"

int MultiMission::On_Which_CD()
{
    return DiskIDs[0];
}
