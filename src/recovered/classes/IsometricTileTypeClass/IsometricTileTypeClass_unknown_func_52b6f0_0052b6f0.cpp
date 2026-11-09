// int IsometricTileTypeClass::unknown_func_52b6f0(int index) (0x0052B6F0)
// The +0x2A byte of sub tile index's cell record in the loaded image (0 without
// an image or a record; hypothesis). Shaped like Is_Randomized 0x0052B730.
#include "shared/isometric_tile_type.hpp"

int IsometricTileTypeClass::unknown_func_52b6f0(int index)
{
    IsoTileImageStruct const *image = (IsoTileImageStruct const *)unknown_slot39();
    if (image != 0) {
        IsoTileCellStruct const *record = (IsoTileCellStruct const *)image->Tiles[index % (image->unknown_00[0] * image->unknown_00[1])];
        if (record != 0) {
            return record->unknown_2a;
        }
    }
    return 0;
}
