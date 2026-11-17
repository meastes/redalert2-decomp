// @ref ??0Cell@@QAE@HH@Z=0042ad30
// @ref ?Can_Place_Paved_Road_00588a30@@YI_NABVRect@@_N1@Z=00588a30
// @ref ?Equal_004fef20@Cell@@QBEHABV1@@Z=004fef20
// @ref ?unknown_generation_cell_clear_0056dac0@seed_0056dac0@@YI_NPAF@Z=0056dac0
// @ref ?Init_Dirt_Roads@@YAXXZ=0058d9d0
// @ref ?In_Local_Radar@MapClass@@QAE_NABVCell@@_N@Z=0055a660
// @ref ?unknown_generation_paths_a700a0@@3PAVDirtRoadTile@@A=00a700a0
// @ref ?IsometricTileTypeClass_DirtRoadSlopes@@3HA=00a6dd04
// @ref ?IsometricTileTypeClass_DirtRoadCurve@@3HA=00a6e014
// @ref __real@8@3ffdfae147ae147ae000=007a6050
// @ref ??AMapClass@@QAEAAVCellClass@@ABVCell@@@Z=00548070
// @ref ??RRandom2Class@@QAEHXZ=00638840
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?Map_Random_Fraction_00579770@@YANXZ=00579770
// @ref ?Map_Random_Range_005797a0@@YIIII@Z=005797a0
// @ref ?Is_Tile_Clear@CellClass@@QBE_NXZ=0047b590
// @ref ?MapCellStride@@3HA=0084e804
// @ref ?RMGCellData@@3PAUUnknownMapGenerationCell@@A=00a70a24
// @ref ?RMGRandom@@3VRandom2Class@@A=00a705c0
// @ref ?unknown_math_4ba520@@YANN@Z=004ba520
// @ref ?unknown_math_4ba5b0@@YANN@Z=004ba5b0
// @ref ?unknown_sqrt_4ba410@@YANN@Z=004ba410
// @ref __CIpow=007821b0
// @ref __ftol=0077f110
// @ref __real@8@3fdf8000000080000000=007a5dd0
// @ref __real@8@3ff78000000000000000=0079a730
// @ref __real@8@3ffe8000000000000000=0079a728
// @ref __real@8@3fff8000000000000000=0079a708
// @ref __real@8@40008000000000000000=0079a6f8
// @ref __real@8@4004b400000000000000=0079a710
// @ref __real@8@4004f000000000000000=0079a718
// @ref __real@8@4005b400000000000000=0079a720
// @ref __real@8@40078000000000000000=0079a700
#include "shared/coord.hpp"
// Original tests X before Y and returns a full 32-bit zero or one.
// Cell has the evidenced two-short layout; the public TPoint base spelling remains qualified.
int Cell::Equal_004fef20(Cell const &other) const
{
    if (X==other.X && Y==other.Y) return 1;
    return 0;
}

// @ref ?Equal_004fef20@Cell@@=004fef20
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
