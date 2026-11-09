// Behaviour row (Scout1111, native M12; NOT EXACT, native/behaviour_rewrites.json):
// 0x0069ED20, comip's out-of-line _AddRef for the IPiggyback pointer (the twin of
// the ILocomotion one, 0x00453EA0, src/recovered/unowned/Locomotor_Add_Ref_Held_00453ea0.cpp:
// the held interface's AddRef when there is one). UnitClass::Assign_Destination
// 0x007053F0 reaches it when the refinery's free harvester docks (harness build order).
#define RA2_BUILDING_LOCOMOTOR_ADDREF_DECL   // UnknownComPtr::_AddRef (shared/com_ptr.hpp)
#include "shared/com_ptr.hpp"
#include "shared/locomotion.hpp"

extern "C" GUID const unknown_uuid_IPiggyback_7cfc30;

template <> void UnknownComPtr<IPiggyback, &unknown_uuid_IPiggyback_7cfc30>::_AddRef() const
{
    if (Pointer != 0) Pointer->AddRef();
}
