// bool IsometricTileClass::Limbo() (0x00528190)
// Takes the tile off the map (only while the 0x00A40958 flag is set).
#include "shared/isometric_tile.hpp"
// @ref ?unknown_flag_a40958@@3_NA=00a40958

bool IsometricTileClass::Limbo()
{
    if (unknown_flag_a40958 && !unknown_76) {
        Unselect();
        unknown_slot54(true);
        Mark(0);
        unknown_76 = true;
        unknown_75 = false;
        return true;
    }
    return false;
}
