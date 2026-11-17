// VectorClass<HashObject<unsigned int, unsigned int> >::VectorClass(unsigned, HashObject<unsigned int, unsigned int>  const *) (0x0056CDD0): the out-of-line copy of the map unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/vector_class.hpp"
#include "shared/hash_object.hpp"


// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
VectorClass<HashObject<unsigned int, unsigned int> >::VectorClass(unsigned size, HashObject<unsigned int, unsigned int>  const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) HashObject<unsigned int, unsigned int> [size];
        } else {
            Vector = new HashObject<unsigned int, unsigned int> [size];
            IsAllocated = true;
        }
    }
}
