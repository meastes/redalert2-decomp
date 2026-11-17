// Forwarder (Scout1103, native M10): AnimClass slot 116 calls the bounce step
// 0x004370E0 through bounce.hpp's first-0x30-bytes view (UnknownBounce); its
// body is the Scout811 draft native/drafts/004370e0.cpp, written as
// BounceClass::AI (the class whose first member is that view). Not original code.
#include "shared/bounce.hpp"

int UnknownBounce::Unknown_004370E0()
{
    return ((BounceClass *)this)->AI();
}
