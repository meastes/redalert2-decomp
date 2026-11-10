// DynamicVectorClass<RGBClass>::ID_object (0x0046DA30; virtual slot 4).
// Layout and behaviour: shared/vector_class.hpp; RGBClass::operator== compares the three bytes (shared/rgb_class.hpp).
#define VECTOR_IMPLEMENT_DYNAMIC_ID_OBJECT
#include "shared/vector_class.hpp"
#include "shared/rgb_class.hpp"

template class DynamicVectorClass<RGBClass>;
