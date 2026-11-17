// DynamicVectorClass<TeamTypeClass const *>::DynamicVectorClass(unsigned, TeamTypeClass const * const *) (0x006C03E0): the out-of-line copy of the teamtype unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/vector_class.hpp"

class TeamTypeClass;

// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
DynamicVectorClass<TeamTypeClass const *>::DynamicVectorClass(unsigned size, TeamTypeClass const * const *array)
    : VectorClass<TeamTypeClass const *>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
