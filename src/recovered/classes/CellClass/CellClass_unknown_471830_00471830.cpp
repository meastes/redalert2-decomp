// ObjectClass *CellClass::unknown_471830(Point2D const &spot, bool alt) (0x00471830): TibSun cell.cpp Cell_Object:
// an aircraft (kind 2) in the occupant list, else the occupant nearest the spot (Cell_Techno), else the terrain
// object (Cell_Terrain). TeamClass 0x006BB260 and Draw_Placement_Cursor 0x00473F20 (which expands it) ask it.
#include "shared/cell.hpp"
#include "shared/unknown_globals.hpp"

ObjectClass *CellClass::unknown_471830(Point2D const &spot, bool alt)
{
    ObjectClass *object;

    object = Find_Object(2);
    if (object != 0) {
        return object;
    }

    object = Cell_Techno(spot, alt, 0);
    if (object != 0) {
        return object;
    }
    object = Cell_Terrain();
    return object;
}
