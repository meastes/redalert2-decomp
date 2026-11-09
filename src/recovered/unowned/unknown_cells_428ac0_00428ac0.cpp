// int __fastcall unknown_cells_428ac0() (0x00428AC0)
// The number of cells of the map's isometric cell block: (height + 4) rows of
// twice the width (TiberiumClass sizes its growth tables with it).
#include "shared/veinhole.hpp"
#include "shared/map_view.hpp"

int __fastcall unknown_cells_428ac0()
{
    return (Map.MapSize.Height + 4) * Map.MapSize.Width * 2;
}
