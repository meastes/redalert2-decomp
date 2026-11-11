// AbstractClass *ObjectClass::Get_Cell_Ptr() (0x005D51E0)
#include "shared/object_type.hpp"
#include "shared/map_view.hpp"

AbstractClass *ObjectClass::Get_Cell_Ptr()
{
    CoordStruct coord = Coord;
    &Map[coord];
    coord = Coord;
    return (AbstractClass *)&Map[coord];   // the cell object (hypothesis)
}
