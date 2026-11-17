// VectorClass<UnitTypeClass *>::VectorClass(unsigned, UnitTypeClass * const *) (0x004FB610): the out-of-line copy of the HouseTypeClass unit
// (shared/vector_class.hpp). The vtable and virtual members it emits are the copies at their recorded addresses.
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/vector_class.hpp"

class UnitTypeClass;

// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
VectorClass<UnitTypeClass *>::VectorClass(unsigned size, UnitTypeClass * const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) UnitTypeClass *[size];
        } else {
            Vector = new UnitTypeClass *[size];
            IsAllocated = true;
        }
    }
}
