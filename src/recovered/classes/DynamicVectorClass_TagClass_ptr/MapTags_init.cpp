// Dynamic initializer of the global MapTags (0x004D4760, _$E): an empty
// DynamicVectorClass<TagClass *> (constructor expanded in place) whose atexit
// destructor (0x004D47A0, the following _$E draft, emitted by this definition)
// expands the vector's destructor. A tag list (shared/map.hpp).
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/vector_class.hpp"
class TagClass;

DynamicVectorClass<TagClass *> MapTags;
