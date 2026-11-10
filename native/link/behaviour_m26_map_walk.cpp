// Behaviour rows (Scout1208, native M26; NOT EXACT): the stores of MapClass 0x0055AA50
// against shared/map.hpp's layout (see behaviour_m26_map.cpp).
#include "shared/map.hpp"

void Native_Map_Start_Second_Walk(MapClass *map)
{
    map->unknown_118 = map->MapLocalSize.X + 1 + map->MapLocalSize.Y;
    map->unknown_11c = map->MapSize.Width - map->MapLocalSize.X + map->MapLocalSize.Y;
}
