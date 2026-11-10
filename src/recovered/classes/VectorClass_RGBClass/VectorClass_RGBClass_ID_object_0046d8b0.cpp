// VectorClass<RGBClass>::ID_object (0x0046D8B0; VectorClass<RGBClass> vtable 0x0079da54 slot 4).
// Layout and behaviour: shared/vector_class.hpp; RGBClass::operator== compares the three bytes (shared/rgb_class.hpp).
#define VECTOR_IMPLEMENT_VECTOR_ID_OBJECT
#include "shared/vector_class.hpp"
#include "shared/rgb_class.hpp"

template class VectorClass<RGBClass>;
