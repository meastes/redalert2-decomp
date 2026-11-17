// Behaviour-written body for native M20 (Scout1161; see behaviour_m20.cpp). NOT EXACT.
#include "shared/wdt_state.hpp"

// 0x0072C390 WDTState::Is_Valid (wdt_state.hpp): its +0x44 is set. The unowned
// view (src/recovered/unowned/unknown_has_44_0072c390.cpp) pads 0x44 bytes.
bool WDTState::Is_Valid() const
{
    return OwnerHistory != 0;
}

