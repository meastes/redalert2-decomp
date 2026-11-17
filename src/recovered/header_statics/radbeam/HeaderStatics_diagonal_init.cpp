#define RA2_RAD_BEAM_TYPE1_UNIT
// Header-static dynamic initializers of the unit at 0x006351B0 and its
// list globals (RadBeamList::Array); the other drafts of this plan are emitted by this file.
// A list whose atexit thunk tests and sets a guard bit is a static data
// member (VC6 guards their destruction); its owner class is a hypothesis.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"
#include "shared/rad_beam_list.hpp"

DynamicVectorClass<RadBeam *> RadBeamList::Array;
