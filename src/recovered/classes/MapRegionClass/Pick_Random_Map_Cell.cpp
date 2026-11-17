#include "shared/coord.hpp"
// Original tests X before Y and returns a full 32-bit zero or one.
// Cell has the evidenced two-short layout; the public TPoint base spelling remains qualified.
int Cell::Equal_004fef20(Cell const &other) const
{
    if (X==other.X && Y==other.Y) return 1;
    return 0;
}

#include "shared/map_region.hpp"
#include "shared/random2.hpp"
unsigned __fastcall Map_Random_Range_005797a0(unsigned minimum,unsigned maximum);
Cell __fastcall MapRegionClass::Pick_Random_Map_Cell()
{
    int gridSize=MapCellStride*MapCellStride;
    unsigned index;
    do { index=Map_Random_Range_005797a0(0,gridSize-1); }
    while((unsigned char)RMGCellData[index].Position.Equal_004fef20(Cell(0,0)));
    return RMGCellData[index].Position;
}
