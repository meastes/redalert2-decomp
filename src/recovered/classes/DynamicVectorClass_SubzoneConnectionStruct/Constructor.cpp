// DynamicVectorClass<SubzoneConnectionStruct>::DynamicVectorClass(unsigned, SubzoneConnectionStruct const *)
// (0x0056AEE0): the out-of-line copy of the Map unit (shared/vector_class.hpp). The vtables and their
// slot functions it emits are the copies kept at their recorded addresses.
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/vector_class.hpp"
#include "shared/map_elements.hpp"

// The generic body of shared/vector_class.hpp, written as an explicit specialization (VC6
// cannot explicitly instantiate a constructor alone; the whole-class instantiation would also
// emit an out-of-line ~DynamicVectorClass the original does not have).
template <>
DynamicVectorClass<SubzoneConnectionStruct>::DynamicVectorClass(unsigned size, SubzoneConnectionStruct const *array)
    : VectorClass<SubzoneConnectionStruct>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
