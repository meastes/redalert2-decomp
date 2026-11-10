// bool CellClass::Is_Tile_Water() const (0x0047A270): the cell's tile belongs to the water set (ra2-decomp cell.cpp
// Is_Tile_Water text; TibSun IS_TILE_IN_SET(WaterSet, WATER_COUNT)). Is_Clear_To_Build 0x004718B0 expands it.
#include "shared/cell.hpp"
#include "shared/tileset_index.hpp"

bool CellClass::Is_Tile_Water() const
{
    return IsoTileIndex >= unknown_tileset_a52580 && IsoTileIndex < unknown_tileset_a52580 + 14;
}
