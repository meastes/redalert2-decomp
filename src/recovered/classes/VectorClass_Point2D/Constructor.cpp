// VectorClass<Point2D>::VectorClass(unsigned, Point2D const *) (0x00635090): the out-of-line copy of the radar unit
// (shared/vector_class.hpp). The vtables and virtual members it emits are the copies at their recorded addresses.
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/vector_class.hpp"
#include "shared/rect.hpp"


// The generic constructor body of shared/vector_class.hpp, written as an explicit specialization
// (VC6 cannot explicitly instantiate a constructor alone).
template <>
VectorClass<Point2D>::VectorClass(unsigned size, Point2D const *array)
    : Vector(0), VectorMax(size), IsValid(true), IsAllocated(false)
{
    if (size) {
        if (array) {
            Vector = new ((void *)array) Point2D[size];
        } else {
            Vector = new Point2D[size];
            IsAllocated = true;
        }
    }
}
