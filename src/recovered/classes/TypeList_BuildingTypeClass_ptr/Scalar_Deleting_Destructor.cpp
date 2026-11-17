// TypeList<BuildingTypeClass *> scalar deleting destructor (0x0058DEC0; vtable 0x007A5E4C slot 0).
// Layout and behaviour: shared/type_list.hpp. ~TypeList is implicit; the
// expanded ~VectorClass stores the VectorClass vtable 0x007A2F08. The
// constructor below only makes the unit emit the vtable and with it this
// destructor; it is the copy at 0x0058DC70.
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/type_list.hpp"

class BuildingTypeClass;

// An empty list growing by 10.
template <>
TypeList<BuildingTypeClass *>::TypeList()
{
}
