// Original voxel software surface provider, replacing the unused-this leaf model.
#include "shared/surface.hpp"
#include "shared/voxel_draw_system.hpp"
Surface *VoxelDrawSystem::Get_Surface()
{
    return &VoxelSurface;
}
