#include "shared/map.hpp"

int MapClass::Get_Cell_Zone_Index(Cell const &cell)
{
    int index = Unknown_0054FAF0(cell);
    int result;
    if (index < 0) {
        result = 0;
    } else if (index >= CellZoneCount) {
        result = CellZoneCount - 1;
    } else {
        result = index;
    }
    return result;
}
