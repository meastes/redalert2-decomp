// Cell const *BuildingTypeClass::Occupy_List(int arg1) const (0x004588C0)
// The foundation's cell list, or an empty list (the end marker alone).
#include "shared/building_type.hpp"
// @ref _?$S48@?1??Occupy_List@BuildingTypeClass@@UBEPBVCell@@H@Z@4EA=0084ed78
// @ref _$E49=00458920
// @ref _?unknown_none@?1??Occupy_List@BuildingTypeClass@@UBEPBVCell@@H@Z@4UUnknownStaticCell@@B=0084edd0
// @ref _atexit=0078298a

Cell const *BuildingTypeClass::Occupy_List(int arg1) const
{
    static UnknownStaticCell const unknown_none(Cell(0x7FFF, 0x7FFF));
    if (FoundationData) {
        return FoundationData;
    }
    return &unknown_none.Cell;
}
