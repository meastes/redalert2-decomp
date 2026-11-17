// Native bridge for the Find_Path_Regular draft (native/drafts/00427370.cpp,
// Scout1103): the map's cell slot array from the full MapClass (map_view.hpp,
// which that draft sees, leaves Cells out). Map (0x008324E0) is the
// MouseClass whose first base is MapClass. Not original code.
#include "shared/map.hpp"

class MouseClass;
extern MouseClass Map;                          // 0x008324E0 (shared/map_view.hpp)

CellClass **Native_Map_Cell_Slot(int index)
{
    return &((MapClass *)(void *)&Map)->Cells[index];
}
