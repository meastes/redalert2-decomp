// Header-static dynamic initializers of the unit at 0x0040F0A0 and the
// unit's WarheadTypeClass list; the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

class WarheadTypeClass;

// The unit's list of WarheadTypeClass objects (0x00839B00; name is a hypothesis).
DynamicVectorClass<WarheadTypeClass *> unknown_WarheadTypeClass_list_00839b00;
#define RA2_WARHEAD_LIST_DEFINED
