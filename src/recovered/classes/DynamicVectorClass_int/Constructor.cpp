// DynamicVectorClass<int>::DynamicVectorClass(unsigned, int const *) (0x0046D1F0): the out-of-line copy of the ccini unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/vector_class.hpp"

// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
DynamicVectorClass<int>::DynamicVectorClass(unsigned size, int const *array)
    : VectorClass<int>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
