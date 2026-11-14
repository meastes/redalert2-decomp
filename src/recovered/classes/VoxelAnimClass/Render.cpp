#include "shared/voxel_anim.hpp"
#include "shared/house.hpp"
#include "shared/map_cell_view.hpp"

bool VoxelAnimClass::Render(Rect *bounds, bool arg2, bool arg3)
{
    CellClass *cell = &Map[Center_Coord()];
    if (cell->unknown_12c & 0x10) {
        unknown_75 = true;
    }
    return ObjectClass::Render(bounds, arg2, false);
}
