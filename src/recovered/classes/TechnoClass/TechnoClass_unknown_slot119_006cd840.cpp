// void TechnoClass::unknown_slot119(int mission, AbstractClass *target, AbstractClass *destination) (0x006CD840, slot 119)
// Remembers +0x228 in +0x22C, then the mission override and slot 220.
#include "shared/techno.hpp"
// @ref ?unknown_slot119@MissionClass@@UAEXHPAVAbstractClass@@0@Z=00594d20

void TechnoClass::unknown_slot119(int mission, AbstractClass *target, AbstractClass *destination)
{
    unknown_22c = unknown_228;
    MissionClass::unknown_slot119(mission, target, destination);
    unknown_slot220(target);
}
