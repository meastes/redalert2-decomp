// Behaviour-written bodies for native M20 (Scout1161). NOT EXACT: each row has no
// accepted source the native tree can use; the body follows the evidence named.
//
// address_aliases.py now refuses to bind a member of an RTTI class to a body
// written against a different, char-padded view of the object (Scout1153's
// RadioClass finding, generalised): the view reaches its fields at their VC6
// offsets, which a vtable pointer and pointer-sized members move natively. The
// three reachable members it unbound are written here against the real classes.
#define VECTOR_IMPLEMENT_DYNAMIC_ACCESSORS
#include "shared/house_screen.hpp"
#include "shared/waypoint_path.hpp"
#include "shared/rules.hpp"

// 0x004F3120 HouseClass (house_screen.hpp; used by ScrollClass 0x00669960 for the
// waypoint-mode cursor): the current waypoint path (+0x154, 0..11) can take one
// more waypoint: its waypoint count (+0x38 = Waypoints' ActiveCount) is below
// Rules MaxWaypointPathLength (+0x98) and no waypoint is selected (+0x24 Selected
// is -1). The unowned view (src/recovered/unowned/unknown_slot_ready_004f3120.cpp)
// pads 0x154 bytes to reach the index.
bool HouseClass::Unknown_004F3120() const
{
    if (unknown_154 < 0 || unknown_154 >= 12) {
        return false;
    }
    WaypointPathClass const *path = WaypointPaths[unknown_154];
    return path->Waypoints.Count() < Rules->unknown_98 && path->Selected == -1;
}
