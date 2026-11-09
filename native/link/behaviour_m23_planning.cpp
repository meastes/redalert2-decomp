// Behaviour row for native M23 (Scout1187). NOT EXACT.
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS   // DynamicVectorClass::Count() inline (vector_class.hpp)
#include "shared/vector_class.hpp"
#include "shared/planning.hpp"

// 0x00613B60 (Leaf_613b60: `return *(this + 0x14)`): techno.hpp's UnknownTechno408 is
// the techno's PlanningTokenClass (planning.hpp); +0x14 is its Nodes count
// (PlanningTokenClass::Get_Node_Count, planning.hpp's inline for the same address).
// Reached by TechnoClass 0x006D4180 / 0x006D1440 for a techno with planned waypoints.
int UnknownTechno408::unknown_613b60()
{
    return ((PlanningTokenClass *)this)->Nodes.Count();
}
