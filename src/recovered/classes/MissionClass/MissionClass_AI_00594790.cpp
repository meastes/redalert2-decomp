// void MissionClass::AI() (0x00594790, slot 23)
// ObjectClass's part; then, for a live object (+0x7D) whose mission timer
// has run out and that has strength left, the slot of the current mission
// (123-148, each returning the frames until the next step) restarts the
// timer. Unknown missions (and 3) use slot 123.
#include "shared/mission.hpp"
// @ref ?AI@ObjectClass@@UAEXXZ=005d27f0
// @ref ?Frame@@3HA=00a40d2c

void MissionClass::AI()
{
    ObjectClass::AI();
    if (unknown_7d && Timer.Value() == 0 && Strength > 0) {
        switch (Mission) {
        case 0:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_SLEEP());
            break;
        case 23:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_HARMLESS());
            break;
        case 7:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_ENTER());
            break;
        case 18:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_CONSTRUCTION());
            break;
        case 19:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_DECONSTRUCTION());
            break;
        case 1:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_ATTACK());
            break;
        case 4:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_RETREAT());
            break;
        case 10:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_HARVEST());
            break;
        case 11:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_GUARD_AREA());
            break;
        case 12:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_RETURN());
            break;
        case 13:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_STOP());
            break;
        case 14:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_AMBUSH());
            break;
        case 16:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_UNLOAD());
            break;
        case 20:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_REPAIR());
            break;
        case 24:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_OPEN());
            break;
        case 22:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_MISSILE());
            break;
        case 5:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_GUARD());
            break;
        case 8:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_CAPTURE());
            break;
        case 9:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_EATEN());
            break;
        case 2:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_MOVE());
            break;
        case 15:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_HUNT());
            break;
        case 21:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_RESCUE());
            break;
        case 25:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_PATROL());
            break;
        case 26:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_PARADROP_APPROACH());
            break;
        case 27:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_PARADROP_OVERFLY());
            break;
        case 28:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_WAIT());
            break;
        case 6:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_GUARD());
            break;
        case 17:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_CAPTURE());
            break;
        default:
            Timer = CDTimerClass<FrameTimerClass>(Do_MISSION_SLEEP());
            break;
        }
    }
}
