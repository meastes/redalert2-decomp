// Dynamic initializer of the global CurrentObject (0x004D4570, _$E): an empty
// DynamicVectorClass<ObjectClass *> (constructor expanded in place) whose atexit
// destructor (0x004D45B0, the following _$E draft, emitted by this definition)
// expands the vector's destructor. Declared in shared/object.hpp.
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#include "shared/vector_class.hpp"
class ObjectClass;

DynamicVectorClass<ObjectClass *> CurrentObject;
