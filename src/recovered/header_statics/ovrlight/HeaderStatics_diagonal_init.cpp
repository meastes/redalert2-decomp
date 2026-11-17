#define RGB_DECLARE_ACCESSORS
// Header-static dynamic initializers of the unit at 0x005DD320 and the
// unit's SpotLightClass list; the other drafts of this plan are emitted by this file.
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#ifndef VECTOR_IMPLEMENT_ALL
#define VECTOR_IMPLEMENT_ALL
#endif
#include "shared/vector_class.hpp"
#include "shared/spot_light.hpp"

// The unit's list of SpotLightClass objects (0x00A731C0; name is a hypothesis).
DynamicVectorClass<SpotLightClass *> SpotLights;
