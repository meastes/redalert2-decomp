// DynamicVectorClass<InfantryTypeClass *>::DynamicVectorClass(unsigned, InfantryTypeClass * const *) (0x004FAA90): the out-of-line copy of the houstype unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/vector_class.hpp"

class InfantryTypeClass;

// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
DynamicVectorClass<InfantryTypeClass *>::DynamicVectorClass(unsigned size, InfantryTypeClass * const *array)
    : VectorClass<InfantryTypeClass *>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
