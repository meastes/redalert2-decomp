// TypeList<BuildingTypeClass *>::TypeList, copy constructor (0x0058EED0; vtable 0x007A5E4C).
// Layout and behaviour: shared/type_list.hpp.
#define VECTOR_IMPLEMENT_VECTOR_COPY_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_ASSIGN
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/type_list.hpp"

class BuildingTypeClass;

// Deep copy of the storage and both counters.
template <>
TypeList<BuildingTypeClass *>::TypeList(TypeList<BuildingTypeClass *> const &list)
    : DynamicVectorClass<BuildingTypeClass *>(list)
{
}
