// Native bridge for the Explosion_Damage draft (native/drafts/0047e2c0.cpp,
// Scout1108): MapClass's low-bridge damage members 0x0055DCA0/0x0055EDB0 (the
// recovered bodies) on the full MapClass; map_view.hpp, which that draft sees,
// does not declare them. Map (0x008324E0) is the MouseClass whose first base
// is MapClass. Not original code.
#include "shared/map.hpp"

class MouseClass;
extern MouseClass Map;                          // 0x008324E0 (shared/map_view.hpp)

bool Native_Map_Damage_Low_Bridge_0055dca0(Cell const &cell)
{
    return ((MapClass *)(void *)&Map)->Unknown_0055DCA0(cell);
}

bool Native_Map_Damage_Low_Bridge_0055edb0(Cell const &cell)
{
    return ((MapClass *)(void *)&Map)->Unknown_0055EDB0(cell);
}
