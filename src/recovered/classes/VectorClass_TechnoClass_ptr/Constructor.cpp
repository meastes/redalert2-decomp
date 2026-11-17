// VectorClass<TechnoClass *>::VectorClass(unsigned, TechnoClass * const *) (0x0040BC10): the out-of-line
// copy (shared/vector_class.hpp). The vtable and the scalar deleting destructor it emits are the
// copies at their recorded addresses (0x0079A7FC; ??_G 0x0040BB70).
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/vector_class.hpp"

class TechnoClass;

// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
VectorClass<TechnoClass *>::VectorClass(unsigned size, TechnoClass * const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) TechnoClass *[size];
        } else {
            Vector = new TechnoClass *[size];
            IsAllocated = true;
        }
    }
}
