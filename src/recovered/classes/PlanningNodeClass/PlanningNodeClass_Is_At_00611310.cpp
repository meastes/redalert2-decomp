#define RA2_COORD_EQUAL_MEMBER
// bool PlanningNodeClass::Is_At(CoordStruct coord) const (0x00611310)
// Natural form (Scout1165 v9): the node's coordinate is fetched into a local and compared.
#include "shared/planning.hpp"

bool PlanningNodeClass::Is_At(CoordStruct coord) const
{
    CoordStruct here;
    here = Coord();
    return here == coord;
}
