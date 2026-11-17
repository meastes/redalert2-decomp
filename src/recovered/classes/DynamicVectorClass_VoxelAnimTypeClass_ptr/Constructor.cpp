// DynamicVectorClass<VoxelAnimTypeClass *>::DynamicVectorClass(unsigned, VoxelAnimTypeClass * const *) (0x00653340): the out-of-line copy of the rules unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/vector_class.hpp"

class VoxelAnimTypeClass;

// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
DynamicVectorClass<VoxelAnimTypeClass *>::DynamicVectorClass(unsigned size, VoxelAnimTypeClass * const *array)
    : VectorClass<VoxelAnimTypeClass *>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
