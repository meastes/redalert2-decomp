// void MapClass::Unknown_0055A1F0() (0x0055A1F0)
// Once per game (player +0x188 clear): passes every cell of the global Map
// with the player to DisplayClass slot 39, then redraws.
#include "shared/map.hpp"
// @ref ?unknown_slot39@DisplayClass@@UAE_NABVCell@@PAVHouseClass@@@Z=0049a370

void MapClass::Unknown_0055A1F0()
{
    if (!unknown_player_a35db4->unknown_188) {
        unknown_player_a35db4->unknown_188 = true;
        Map.Reset_Iterator();
        for (CellClass *cell = Map.Iterate(); cell != 0; cell = Map.Iterate()) {
            Map.unknown_slot39(cell->MapCoords, unknown_player_a35db4);
        }
        Map.Complete_Radar_Refresh();
        Map.Flag_To_Redraw(1);
    }
}
