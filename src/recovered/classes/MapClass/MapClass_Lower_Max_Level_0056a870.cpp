// void MapClass::Lower_Max_Level(int value) (0x0056A870)
// Lowers +0x144 to value when it is higher, and keeps it from going below 0.
#include "shared/map.hpp"

void MapClass::Lower_Max_Level(int value)
{
    unknown_144 = unknown_144 < value ? unknown_144 : value;
    unknown_144 = unknown_144 > 0 ? unknown_144 : 0;
}
