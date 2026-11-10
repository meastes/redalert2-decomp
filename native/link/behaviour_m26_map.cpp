// Behaviour rows (Scout1208, native M26; NOT EXACT, native/behaviour_rewrites.json).
// 0x0055AA50 (pass-6a decompile): starts the second cell walk (0x0055A910) at the
// local map's first diagonal: +0x118 = MapLocalSize.X + 1 + MapLocalSize.Y, +0x11C =
// MapSize.Width - MapLocalSize.X + MapLocalSize.Y. The member is declared only by
// shared/map_view.hpp, whose MapClass view keeps +0x108..+0x11F as one char run; natively
// that run's layout differs from shared/map.hpp's (a pointer at +0x114), and the walk
// 0x0055A910 reads map.hpp's unknown_118/11c. So the stores are made by a helper compiled
// against shared/map.hpp (behaviour_m26_map_walk.cpp).
#include "shared/map_view.hpp"

void Native_Map_Start_Second_Walk(MapClass *map);    // behaviour_m26_map_walk.cpp

void MapClass::Unknown_0055AA50()
{
    Native_Map_Start_Second_Walk(this);
}
