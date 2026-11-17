// VectorClass<ScoutStruct>::VectorClass(unsigned, ScoutStruct const *) (0x004F8B80): the out-of-line copy of the house unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/vector_class.hpp"
#include "shared/house_nodes.hpp"


// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
VectorClass<ScoutStruct>::VectorClass(unsigned size, ScoutStruct const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) ScoutStruct[size];
        } else {
            Vector = new ScoutStruct[size];
            IsAllocated = true;
        }
    }
}
