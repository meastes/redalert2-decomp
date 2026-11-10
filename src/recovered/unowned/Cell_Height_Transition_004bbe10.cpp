#include "shared/cell.hpp"
// The cliff test of the projectile path (inlined into Get_Obstacle_004bbaf0, as the gamemd twin
// TrajectoryHelper::IsCliffHit in ra2-decomp src/findpath2.cpp; see THIRD_PARTY.md): the cell rises four or more
// levels over the previous cell and above the origin. CellClass 0x0047CF10 returns +0x11B plus four for flag 0x80.
// @ref ?Bridge_Adjusted_Height_0047CF10@CellClass@@QBEHXZ=0047cf10
int __fastcall Cell_Height_Transition_004bbe10(CellClass const *origin,
                                             CellClass const *nearby,
                                             CellClass const *destination)
{
    return destination->Bridge_Adjusted_Height_0047CF10() - nearby->Bridge_Adjusted_Height_0047CF10() >= 4
        && destination->Bridge_Adjusted_Height_0047CF10() - origin->Bridge_Adjusted_Height_0047CF10() > 0;
}
