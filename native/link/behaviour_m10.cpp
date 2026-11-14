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
// 0x004CD9B0 FootClass (seed leaf): clears +0x490 and +0x494.
void FootClass::unknown_4cd9b0()
{
    unknown_490 = 0;
    unknown_494 = 0;
}

// 0x004131D0 AircraftTrackerClass (seed leaf Leaf_4131d0::get: `ret 4`, an
// empty body): a detached object needs nothing here.
void AircraftTrackerClass::Detach_004131d0(AbstractClass *target)
{
}
