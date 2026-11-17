// DynamicVectorClass<UnitTypeClass *>::DynamicVectorClass(unsigned, UnitTypeClass * const *) (0x004FAE60): the out-of-line copy of the houstype unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/vector_class.hpp"

class UnitTypeClass;

// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
DynamicVectorClass<UnitTypeClass *>::DynamicVectorClass(unsigned size, UnitTypeClass * const *array)
    : VectorClass<UnitTypeClass *>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
