// TypeList<BuildingTypeClass *>::TypeList, default constructor (0x0058DC70; vtable 0x007A5E4C).
// Layout and behaviour: shared/type_list.hpp.
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/type_list.hpp"

class BuildingTypeClass;

// An empty list growing by 10.
template <>
TypeList<BuildingTypeClass *>::TypeList()
{
}
