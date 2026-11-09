// wchar_t const *PlanningModeCommandClass::Get_Description() const -- 0x0051B990
// The localized description (Cmnd:PlanningModeDesc).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BG@MPBI@Cmnd?3PlanningModeDesc?$AA@=007dcbf8

wchar_t const *PlanningModeCommandClass::Get_Description() const
{
    return StringTable::Load_String("Cmnd:PlanningModeDesc", 0, "D:\\Ra2\\Init.CPP", 6329);
}
