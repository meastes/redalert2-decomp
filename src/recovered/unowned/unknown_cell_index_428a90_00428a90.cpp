// int __fastcall unknown_cell_index_428a90(Cell const &cell) (0x00428A90)
// The index of a cell in the map's isometric cell block (0x00428AC0 counts them).
#include "shared/veinhole.hpp"
#include "shared/map_view.hpp"

int __fastcall unknown_cell_index_428a90(Cell const &cell)
{
    return (cell.X - Map.MapSize.Width - 1 + cell.Y) * Map.MapSize.Width + ((cell.X - cell.Y - 1 + Map.MapSize.Width) >> 1);
}
