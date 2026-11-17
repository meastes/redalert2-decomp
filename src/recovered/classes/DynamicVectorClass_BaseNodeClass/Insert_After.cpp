// bool DynamicVectorClass<BaseNodeClass>::Insert_After(BaseNodeClass const &object, int index) (0x004F7270)
// The out-of-line copy of the insert-after-index growth/shift code (Base.cpp
// side; called by the 0x004F61D0 base planner).
#define VECTOR_IMPLEMENT_DYNAMIC_INSERT_AFTER
#define VECTOR_IMPLEMENT_VECTOR_ACCESSORS   // Length() in the growth test
#include "shared/vector_class.hpp"
#include "shared/house_nodes.hpp"

template class DynamicVectorClass<BaseNodeClass>;
