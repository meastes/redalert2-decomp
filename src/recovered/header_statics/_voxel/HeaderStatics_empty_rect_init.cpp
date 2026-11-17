// Header-static dynamic initializers of the unit at 0x0040EA30 and the
// unit's VoxelAnimClass list; the other drafts of this plan are emitted by this file.
#include "shared/shape_file.hpp"
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

class VoxelAnimClass;

// The unit's list of VoxelAnimClass objects (0x008399D0; name is a hypothesis).
DynamicVectorClass<VoxelAnimClass *> VoxelAnims;
