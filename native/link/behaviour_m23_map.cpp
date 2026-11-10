// Behaviour rows for native M23 (Scout1187). NOT EXACT.
#include "shared/display.hpp"     // before map_view.hpp (full DisplayClass)
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"

// BuildingClass::Unlimbo 0x0043D3A0's name for the map's object lookup.
class UnknownCellObjectMap {
public:
    ObjectClass *Cell_Object(Cell const &cell, Point2D const &offset);
};

// 0x0049A5B0 (pass-6a decompile; TibSun DisplayClass::Cell_Object): the occupant of
// the cell nearest the spot, CellClass 0x00471830 with the flag false. Reached by
// Unlimbo when an upgrade (PowersUpBuilding) is placed on its building.
ObjectClass *UnknownCellObjectMap::Cell_Object(Cell const &cell, Point2D const &offset)
{
    return Map[cell].Cell_Object(offset, false);
}
