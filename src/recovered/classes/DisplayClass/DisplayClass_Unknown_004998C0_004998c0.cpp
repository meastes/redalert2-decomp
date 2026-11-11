// void DisplayClass::Unknown_004998C0(Cell const &cell, bool set) (0x004998C0)
// Sets (or clears) flag 2 of every cell of the list at +0x1178 placed at
// the given cell, for the cells on the map.
#include "shared/display.hpp"
#include "shared/map_cell.hpp"
// @ref ?In_Radar@MapClass@@QAE_NABVCell@@@Z=0054aab0
// @ref ??AMapClass@@QAEAAVCellClass@@ABVCell@@@Z=00548070
// @ref _unknown_geometry_k=00852610

void DisplayClass::Unknown_004998C0(Cell const &cell, bool set)
{
    if (cell.X == unknown_geometry_k.X && cell.Y == unknown_geometry_k.Y) {
        return;
    }
    Cell const *offset = unknown_1178;
    while (*offset != Cell(0x7FFF, 0x7FFF)) {
        Cell target = cell + *offset++;
        if (In_Radar(target)) {
            CellClass &target_cell = (*this)[target];
            if (set) {
                target_cell.unknown_12c = target_cell.unknown_12c | 2;
            } else {
                target_cell.unknown_12c = target_cell.unknown_12c & ~2;
            }
        }
    }
}
