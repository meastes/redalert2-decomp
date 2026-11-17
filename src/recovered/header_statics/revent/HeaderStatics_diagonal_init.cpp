// Header-static dynamic initializers of the unit at 0x0063B300 and its
// list globals (RadarEventClass); the other drafts of this plan are emitted by this file.
// A list whose atexit thunk tests and sets a guard bit is a static data
// member (VC6 guards their destruction); its owner class is a hypothesis.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"

// Only the members this unit defines; the layout is not recovered here.
class RadarEventClass {
public:
    static DynamicVectorClass<RadarEventClass *> Array;   // 0x00AB67E8
};

DynamicVectorClass<RadarEventClass *> RadarEventClass::Array;
