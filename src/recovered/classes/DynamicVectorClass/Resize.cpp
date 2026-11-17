// bool DynamicVectorClass<ZoneConnectionClass>::Resize(int newsize, ZoneConnectionClass const *array)
// (0x0056B330, vtable 0x007A59F8 slot 2). The template instance alone; the
// element constructor's Cell members expand through the charged Cell
// constructor, whose budget runs out at To (0x0042AD30 out of line).
#define VECTOR_IMPLEMENT_DYNAMIC_RESIZE
#include "shared/vector_class.hpp"
#include "shared/map_elements.hpp"

template class DynamicVectorClass<ZoneConnectionClass >;
