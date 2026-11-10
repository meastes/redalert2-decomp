// void AircraftClass::Override_Mission(int mission, AbstractClass *target, AbstractClass *destination) (0x0041B1A0, slot 119)
// FootClass's override of the mission, except that missions 4, 26 and 27
// are only replaced by one another.
#include "shared/aircraft.hpp"
// @ref ?Override_Mission@FootClass@@UAEXHPAVAbstractClass@@0@Z=004c7ac0

void AircraftClass::Override_Mission(int mission, AbstractClass *target, AbstractClass *destination)
{
    if ((Mission == 4 || (Mission > 25 && Mission <= 27)) && mission != 4 && (mission <= 25 || mission > 27)) {
        return;
    }
    FootClass::Override_Mission(mission, target, destination);
}
