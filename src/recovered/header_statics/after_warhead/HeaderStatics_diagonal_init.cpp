// Header-static dynamic initializers of the unit at 0x0040F720 and the
// unit's WeaponTypeClass list; the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

class WeaponTypeClass;

// The unit's list of WeaponTypeClass objects (0x00839B90; name is a hypothesis).
DynamicVectorClass<WeaponTypeClass *> Weapons;
