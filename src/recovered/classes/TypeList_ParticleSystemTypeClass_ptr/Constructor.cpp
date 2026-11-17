// TypeList<ParticleSystemTypeClass *>::TypeList, copy constructor (0x006DF9E0; vtable 0x007AD144).
// Layout and behaviour: shared/type_list.hpp.
#define VECTOR_IMPLEMENT_VECTOR_COPY_CONSTRUCTOR
#define VECTOR_IMPLEMENT_VECTOR_ASSIGN
#define VECTOR_IMPLEMENT_VECTOR_DESTRUCTOR
#include "shared/type_list.hpp"

class ParticleSystemTypeClass;

// Deep copy of the storage and both counters.
template <>
TypeList<ParticleSystemTypeClass *>::TypeList(TypeList<ParticleSystemTypeClass *> const &list)
    : DynamicVectorClass<ParticleSystemTypeClass *>(list)
{
}
