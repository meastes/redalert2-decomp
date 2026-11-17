// Header-static dynamic initializers of the unit at 0x0040B280:  the
// shared/unknown_geometry.hpp constants (0x0040B280-0x0040B441); the other
// drafts of this plan are emitted by these includes; BombList's definition below emits
// its initializer 0x0040B450 and atexit destructor 0x0040B4D0.
#include "shared/unknown_geometry.hpp"
#define VECTOR_IMPLEMENT_DYNAMIC_CONSTRUCTOR
#define BOMB_LIST_DECLARE_LIFETIME
#include "shared/vector_class.hpp"
#include "shared/saveload_lists.hpp"

// The planted bombs and the BombSight units (0x008322E0).
BombListClass BombList;
