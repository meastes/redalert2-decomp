// DynamicVectorClass<TechnoClass *>::DynamicVectorClass(unsigned size, TechnoClass *const *array) (0x006F8A80; vtable 0x0079A79C)
// The sized constructor kept in this unit: VectorClass storage (the array in
// place, or allocated and owned), growth step 10, no active entries.
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/vector_class.hpp"

class TechnoClass;

template <>
DynamicVectorClass<TechnoClass *>::DynamicVectorClass(unsigned size, TechnoClass *const *array)
    : VectorClass<TechnoClass *>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
