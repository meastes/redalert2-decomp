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

// 0x00617A00 (Scout1281; Leaf_617a00 is the seed's raw read of 0x00A766B0): returns the node
// the planning mode remembers at 0x00A766B0 (mov eax, [0x00A766B0]; ret). The Delete command
// (DeleteCommandClass::Execute 0x0051DA30 -> unknown_dispatch 0x006F6490) deletes that node,
// else falls back; it was a link-closure trap. unknown_dispatch declares it with its own
// stand-in return type; the symbol (return type not mangled) is the same.
PlanningNodeClass *unknown_find_617a00()
{
    return unknown_node_a766b0;
}
