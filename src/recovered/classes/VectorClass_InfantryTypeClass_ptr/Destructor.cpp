// VectorClass<InfantryTypeClass *>::~VectorClass (0x00652840; stores the VectorClass<InfantryTypeClass *> vtable 0x007a3118).
// Layout and behaviour: shared/vector_class.hpp.
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/vector_class.hpp"

class InfantryTypeClass;

template class VectorClass<InfantryTypeClass *>;
