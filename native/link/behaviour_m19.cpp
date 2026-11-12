// Behaviour-written bodies for native M19 (Scout1153). NOT EXACT: each row has no
// accepted source the native tree can use; the body follows the evidence named.
#include "shared/radio.hpp"

// 0x0047A270 CellClass::Is_Tile_Water (src/shared/map_cell.hpp; pass-6a decompile and the
// row's seed src/recovered/0047a270.cpp): the cell's isometric tile index lies in the
// 14 tiles from the water tile set's first index (0x00A52580). Reached by
// SuperClass::Place 0x0069D850 when the computer fires a superweapon (Scout1153's
// El Dorado win run, frame ~30,000).
#include "shared/map_cell.hpp"
#include "shared/tileset_index.hpp"
bool CellClass::Is_Tile_Water() const
{
    return unknown_tileset_a52580 <= IsoTileIndex && IsoTileIndex < unknown_tileset_a52580 + 14;
}

// 0x00423EF0 under its second declared name (shared/facing.hpp: TibSun DirType::Direction,
// "the same code as the constructor above"): the direction from one coordinate to another,
// stored in this direction, which is returned. Reached by AnimClass::Flaming_Guy_AI
// (Allied 1, a burning infantryman).
#include "shared/facing.hpp"
UnknownFacingDir &UnknownFacingDir::Direction(CoordStruct const &from, CoordStruct const &to)
{
    *this = UnknownFacingDir(from, to);
    return *this;
}
