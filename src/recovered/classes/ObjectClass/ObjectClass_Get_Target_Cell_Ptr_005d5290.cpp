// CellClass *ObjectClass::Get_Target_Cell_Ptr() const (0x005D5290)
#include "shared/object_type.hpp"
#include "shared/map_view.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0

CellClass *ObjectClass::Get_Target_Cell_Ptr() const
{
    return &Map[Coord_Cell(unknown_slot19(0))];
}
