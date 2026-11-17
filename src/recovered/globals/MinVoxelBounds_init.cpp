// Dynamic initializer of the global Vector3 0x00AE0A70 (0x00716EA0): empty,
// from Vector3's empty constructor. The voxel code resets it to
// (10000, 10000, 10000) and lowers it to the smallest vertex coordinates seen
// (bounding box minimum, hypothesis).
#include "shared/vector3.hpp"

Vector3 MinVoxelBounds;
