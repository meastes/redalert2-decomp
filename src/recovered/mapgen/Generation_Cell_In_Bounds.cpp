#include "shared/coord.hpp"
extern int MapRegionClass_MapStartDiagonal,MapRegionClass_MapEndDiagonal;
// @ref ?MapRegionClass_MapStartDiagonal@@3HA=00a70a1c
// @ref ?MapRegionClass_MapEndDiagonal@@3HA=00a70a20
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &cell)
{
    int x=cell.X;
    int y=cell.Y;
    return (x+y>MapRegionClass_MapStartDiagonal && x-y<MapRegionClass_MapStartDiagonal &&
           y-x<MapRegionClass_MapStartDiagonal && x+y<=MapRegionClass_MapEndDiagonal) ? true : false;
}
