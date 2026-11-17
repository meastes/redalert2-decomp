// Behaviour row (Scout1133, native M16; NOT EXACT, native/behaviour_rewrites.json).
// UnknownComPtr<UnknownFlyInterface>::Release_Held: the comip _Release copy of
// the aircraft's fly-interface pointer (FlyLocomotionClass::Process_Take_Off
// 0x004BDDF0 through UnknownFlyPtr's converting query); as every recovered
// copy (IPiggyback 0x00509860, IPersistStream 0x004A4D10): releases what is held.
// The macros are the Fly.cpp population's (its first lines).
#define RA2_SHIP_MOVE_TO_VALUE
#define RA2_COORD_AS_CELL
#define RA2_COORD_TPOINT3D
#include "shared/unknown_geometry.hpp"
#include "shared/unknown_static_rect.hpp"
#include "shared/fly_locomotion.hpp"
template <> void UnknownComPtr<UnknownFlyInterface, &unknown_uuid_7d80a8>::Release_Held()
{
    if (Pointer != 0) {
        Pointer->Release();
    }
}
