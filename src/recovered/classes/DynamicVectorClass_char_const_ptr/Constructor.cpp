// DynamicVectorClass<char const *>::DynamicVectorClass(unsigned size, char const *const *array) (0x006F8AE0; vtable 0x007A65E4)
// The sized constructor kept in this unit: VectorClass storage (the array in
// place, or allocated and owned), growth step 10, no active entries.
#define VECTOR_IMPLEMENT_VECTOR_CONSTRUCTOR
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/vector_class.hpp"



template <>
DynamicVectorClass<char const *>::DynamicVectorClass(unsigned size, char const *const *array)
    : VectorClass<char const *>(size, array)
{
    GrowthStep = 10;
    ActiveCount = 0;
}
