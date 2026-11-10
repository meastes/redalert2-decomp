// Behaviour-written rows the skirmish harness reaches once units take move
// and deploy orders (Scout1103, native M10). NOT EXACT; never original code or bytes.
#include "shared/com_ptr.hpp"
#include "shared/locomotion.hpp"

extern "C" GUID const unknown_uuid_ILocomotion_7ce600;

// 0x004CDE90 (seed decompile: comip's checked operator->): the held
// locomotor, E_POINTER through _com_issue_error when empty.
ILocomotion *__fastcall Unit_Checked_Locomotor_004cde90(UnknownComPtr<ILocomotion, &unknown_uuid_ILocomotion_7ce600> const &owner)
{
    return owner.Checked();
}

#include "shared/foot.hpp"

// 0x004C7AC0 FootClass slot 119 (the mission override; accepted only as the
// seed leaf Leaf_4c7ac0::get, whose view the native link cannot call): the
// navigation target (+0x494) is remembered in +0x498, TechnoClass's override
// runs, then slot 260 (Assign_Destination) takes the destination, immediate.
// TibSun FootClass::Override_Mission has the same shape. The address alias
// that bound this name to TechnoClass's body skipped both stores.
void FootClass::Override_Mission(int mission, AbstractClass *target, AbstractClass *destination)
{
    unknown_498 = unknown_494;
    TechnoClass::Override_Mission(mission, target, destination);
    Assign_Destination(destination, true);
}

#include "shared/foot.hpp"
// 0x004CD9B0 FootClass (seed leaf): clears +0x490 and +0x494.
void FootClass::unknown_4cd9b0()
{
    unknown_490 = 0;
    unknown_494 = 0;
}

// 0x004131D0 UnknownTracker839EA0 (seed leaf Leaf_4131d0::get: `ret 4`, an
// empty body): a detached object needs nothing here.
void UnknownTracker839EA0::Unknown_004131D0(AbstractClass *target)
{
}
