// Header-static dynamic initializers of the unit at 0x0040E3C0 and the
// unit's ScriptClass list; the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

class ScriptClass;

// The unit's list of ScriptClass objects (0x00839900; name is a hypothesis).
DynamicVectorClass<ScriptClass *> unknown_ScriptClass_list_00839900;
