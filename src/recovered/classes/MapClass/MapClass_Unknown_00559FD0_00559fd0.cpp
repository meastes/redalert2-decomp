// void MapClass::Unknown_00559FD0() (0x00559FD0)
// Clears every cell's visibility bits (+0x12C bits 3-4, +0x140 bits 0-1),
// runs DisplayClass 0x0049E110 (false, false), clears the player's +0x188,
// redraws the radar and flags a full redraw.
#include "shared/map.hpp"
#include "shared/map_cell.hpp"
#include "shared/house_screen.hpp"
#include "shared/map_view.hpp"
// @ref ?Map@@3VMouseClass@@A=008324e0
// @ref ?unknown_player_a35db4@@3PAVHouseClass@@A=00a35db4
// @ref ?Unknown_0049E110@DisplayClass@@QAEX_N0@Z=0049e110
// @ref ?Complete_Radar_Refresh@RadarClass@@QAEXXZ=00634030

void MapClass::Unknown_00559FD0()
{
    Reset_Iterator();
    for (CellClass *cell = Iterate(); cell != 0; cell = Iterate()) {
        cell->IsMapped = false; cell->IsVisible = false;
        cell->IsFogVisible = false; cell->IsFogMapped = false;
    }
    Map.Unknown_0049E110(false, false);
    unknown_player_a35db4->unknown_188 = false;
    Map.Complete_Radar_Refresh();
    Flag_To_Redraw(2);
}
