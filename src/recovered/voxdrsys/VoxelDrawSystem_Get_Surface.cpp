// Original voxel software surface provider, replacing the unused-this leaf model.
#include "shared/surface.hpp"
#include "shared/voxel_draw_system.hpp"
// @ref ?VoxelSurface@@3VBSurface@@A=00ae0db8
Surface *VoxelDrawSystem::Get_Surface()
{
    return &VoxelSurface;
}
