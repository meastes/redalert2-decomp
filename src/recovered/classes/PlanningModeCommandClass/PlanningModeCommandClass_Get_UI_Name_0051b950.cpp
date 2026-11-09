// wchar_t const *PlanningModeCommandClass::Get_UI_Name() const -- 0x0051B950
// The localized name (Cmnd:PlanningMode).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BC@DCHE@Cmnd?3PlanningMode?$AA@=007dcbd8

wchar_t const *PlanningModeCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("Cmnd:PlanningMode", 0, "D:\\Ra2\\Init.CPP", 6325);
}
