// DynamicVectorClass<AircraftTypeClass *>::DynamicVectorClass(unsigned, AircraftTypeClass * const *) (0x004FB230): the out-of-line copy of the houstype unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/vector_class.hpp"

class AircraftTypeClass;

// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
DynamicVectorClass<AircraftTypeClass *>::DynamicVectorClass(unsigned size, AircraftTypeClass * const *array)
    : VectorClass<AircraftTypeClass *>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
