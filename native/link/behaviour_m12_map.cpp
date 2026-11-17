// Native bridge for the Random_Cell_In_Zone draft (native/drafts/004eca50.cpp,
// Scout1111): MapClass::Clip_To_Map 0x00568D60 (the recovered body) on the full
// MapClass; map_view.hpp, which that draft sees, does not declare it. Map
// (0x008324E0) is the MouseClass whose first base is MapClass. Not original code.
#include "shared/map.hpp"

class MouseClass;
extern MouseClass Map;                          // 0x008324E0 (shared/map_view.hpp)

Cell Native_Map_Clip_To_Map_00568d60(Cell const &cell)
{
    return ((MapClass *)(void *)&Map)->Clip_To_Map(cell);
}
