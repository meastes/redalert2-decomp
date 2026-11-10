// Behaviour rows (Scout1208, native M27; NOT EXACT, native/behaviour_rewrites.json).
// Reached by the skirmish Random Map's Generate Map.
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/vector_class.hpp"
#include "shared/coord.hpp"
#include "shared/rect.hpp"
#include "shared/map_region.hpp"
#include "shared/isometric_tile_type.hpp"

// 0x0056DBA0 / 0x0056DBC0 (pass-6a decompiles; ra2-decomp MapRegionClass::Get_Cell_Data, YR
// 0x0058C2A0/0x0058C2C0): the generator's record of a cell (by X + Y * side) and of a cell index.
UnknownMapGenerationCell &__fastcall Get_Cell_Data_0056dba0(Cell const &cell)
{
    return unknown_map_cells_a70a24[cell.X + cell.Y * unknown_map_cell_side_84e804];
}
UnknownMapGenerationCell &__fastcall Get_Cell_Data_0056dbc0(int index)
{
    return unknown_map_cells_a70a24[index];
}

// 0x0052E390 IsometricTileTypeClass::Preview_Tile (pass-6a decompile 0052e390; TibSun
// Preview_Tile): loads the image on demand (unknown_a4 null and the on-demand flag set:
// 0x0052B560), then the colour pair of sub tile `tile` at light level `level`.
unsigned short *IsometricTileTypeClass::Preview_Tile(int tile, int level)
{
    if (unknown_a4 == 0 && unknown_2c4) {
        unknown_func_52b560();
    }
    return unknown_274[tile] + level * 2;
}

// 0x0056DBA0 under the name the deformation-grid rows use (a pointer to the same record).
UnknownMapGenerationCell *__fastcall unknown_generation_cell_56dba0(Cell const &cell)
{
    return &unknown_map_cells_a70a24[cell.X + cell.Y * unknown_map_cell_side_84e804];
}
