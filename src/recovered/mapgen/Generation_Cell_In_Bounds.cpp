#include "shared/coord.hpp"
extern int MapRegionClass_MapStartDiagonal,MapRegionClass_MapEndDiagonal;
bool __fastcall Generation_Cell_In_Bounds_0058da10(Cell const &cell)
{
    int x=cell.X;
    int y=cell.Y;
    return (x+y>MapRegionClass_MapStartDiagonal && x-y<MapRegionClass_MapStartDiagonal &&
           y-x<MapRegionClass_MapStartDiagonal && x+y<=MapRegionClass_MapEndDiagonal) ? true : false;
}
