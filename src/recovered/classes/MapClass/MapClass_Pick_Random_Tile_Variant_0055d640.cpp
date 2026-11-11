// bool MapClass::Pick_Random_Tile_Variant(int first, int last, int height, int region_id, bool &result,
//     bool set_region) (0x0055D640)
// Places the pending isometric tile (Map +0x11A4) at the zone cell (Map +0x1170): every subtile cell inside the
// playfield diamond with a record is checked against the map generator's region (0x005818A0; set_region claims
// cell region 0). A foreign region allows only a matching shore group (0x7E0304) on a non-clear cell; the own region
// refuses shore facings 3-5 apart (0x7E03AC); a clear cell joins the region (0x00581870); an occupied unclaimed
// cell refuses unless region_id is -1 and is then skipped. Clear cells and tiles in [first, last] take the subtile;
// otherwise placement fails, setting result false only on a shore-vs-cliff conflict.
// Shape after TibSun MapClass::Pick_Random_Tile_Variant (map.cpp), our text; RA2 adds set_region and ends the
// unclaimed occupied arm with a `place` flag where TibSun has `continue` (the flag test is jump-threaded, which
// lays that arm after the shared exit as in the original).
#include "shared/map.hpp"
#include "shared/map_view.hpp"
#include "shared/map_cell.hpp"
#include "shared/map_isotile.hpp"
#include "shared/unknown_map_generator.hpp"
#include "shared/tileset_index.hpp"
extern int unknown_shore_groups_7e0304[42];
extern int unknown_shore_facings_7e03ac[42];
// @ref ?unknown_shore_groups_7e0304@@3PAHA=007e0304
// @ref ?unknown_shore_facings_7e03ac@@3PAHA=007e03ac
// @ref ?unknown_47b5e0@CellClass@@QBE_NXZ=0047b5e0
// @ref ?unknown_47b7c0@CellClass@@QBE_NXZ=0047b7c0
// @ref ?Get_Cell_Data_Region@MapGeneratorClass@@QAEHABVCell@@@Z=005818a0
// @ref ?Set_Cell_Data_Region@MapGeneratorClass@@QAEXABVCell@@H@Z=00581870
struct IsoTileCellHeight {
    char unknown_00[0x28];
    char Height;
};

bool MapClass::Pick_Random_Tile_Variant(int first, int last, int height, int region_id, bool &result, bool set_region)
{
    if (((AbstractClass *)Map.unknown_11a4)->Kind_Of() == 18) {
        if (set_region) region_id = 0;
        IsometricTileTypeClass *type = (IsometricTileTypeClass *)Map.unknown_11a4;
        IsoTileImageStruct const *image = (IsoTileImageStruct const *)type->Get_Image_Data();
        if (image != 0) {
            for (int row = 0; row < type->unknown_2b8; ++row) {
                for (int column = 0; column < type->unknown_2b4; ++column) {
                    Cell cell = XY_Cell(column + Map.unknown_1170.X, row + Map.unknown_1170.Y);
                    if (cell.X + cell.Y <= Map.MapSize.Width || cell.X - cell.Y >= Map.MapSize.Width ||
                        cell.Y - cell.X >= Map.MapSize.Width || cell.X + cell.Y > Map.MapSize.Width + 2 * Map.MapSize.Height)
                        continue;
                    CellClass *cellptr = &Map[cell];
                    int subtile = column + type->unknown_2b4 * row;
                    IsoTileCellHeight const *record = (IsoTileCellHeight const *)image->Tiles[subtile];
                    if (record == 0) continue;
                    int id = RandomMapGen.Get_Cell_Data_Region(cell);
                    if (set_region) id = 0;
                    bool place = true;
                    if (id > 0 && id != region_id && region_id != -1) {
                        int existing = cellptr->IsoTileIndex - unknown_tileset_a6cb74;
                        int pending = type->ArrayIndex - unknown_tileset_a6cb74;
                        if (!cellptr->unknown_47b590()) {
                            if (existing < 0 || pending < 0 || existing > 41 || pending > 41 ||
                                unknown_shore_groups_7e0304[existing] != unknown_shore_groups_7e0304[pending]) {
                                result = false;
                                return false;
                            }
                            return true;
                        }
                        RandomMapGen.Set_Cell_Data_Region(cell, region_id);
                    } else if (id == region_id) {
                        int existing = cellptr->IsoTileIndex - unknown_tileset_a6cb74;
                        int pending = type->ArrayIndex - unknown_tileset_a6cb74;
                        if (existing >= 0 && pending >= 0 && existing <= 41 && pending <= 41) {
                            int difference = abs(unknown_shore_facings_7e03ac[existing] - unknown_shore_facings_7e03ac[pending]);
                            if (difference >= 3 && difference <= 5) {
                                result = false;
                                return false;
                            }
                        }
                    } else if (cellptr->unknown_47b590()) {
                        RandomMapGen.Set_Cell_Data_Region(cell, region_id);
                    } else if (region_id != -1) {
                        result = false;
                        return false;
                    } else {
                        place = false;
                    }
                    if (place) {
                        if (cellptr->unknown_47b590() || (cellptr->IsoTileIndex >= first && cellptr->IsoTileIndex <= last)) {
                            cellptr->IsoTileIndex = type->ArrayIndex;
                            cellptr->unknown_11a = subtile;
                            cellptr->unknown_11b = height + record->Height;
                        } else {
                            if ((cellptr->unknown_47b7c0() && Unknown_0055AF80(type->ArrayIndex, subtile)) ||
                                (type->ArrayIndex >= unknown_tileset_a6cb74 && type->ArrayIndex < unknown_tileset_a6cb74 + 42 &&
                                 cellptr->unknown_47b5e0()))
                                result = false;
                            return false;
                        }
                    }
                }
            }
            return true;
        }
    }
    return false;
}
