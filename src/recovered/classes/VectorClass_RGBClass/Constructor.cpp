// VectorClass<RGBClass>::VectorClass(unsigned, RGBClass const *) (0x0046DC90): the out-of-line copy of the ccini unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/vector_class.hpp"
#include "shared/rgb_class.hpp"


// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
VectorClass<RGBClass>::VectorClass(unsigned size, RGBClass const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) RGBClass[size];
        } else {
            Vector = new RGBClass[size];
            IsAllocated = true;
        }
    }
}
