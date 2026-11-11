// bool IsometricTileClass::Unlimbo(CoordStruct const *coord, int dir) (0x00528160)
// Places the tile at the coordinate and marks it mapped (slot 71).
#include "shared/isometric_tile.hpp"

bool IsometricTileClass::Unlimbo(CoordStruct const *coord, int dir)
{
    unknown_76 = false;
    Set_Coord(*coord);
    return Mark(1);
}
