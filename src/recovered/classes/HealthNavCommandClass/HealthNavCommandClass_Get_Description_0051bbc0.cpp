// wchar_t const *HealthNavCommandClass::Get_Description() const -- 0x0051BBC0
// The localized description (Cmnd:HealthNavigationDesc).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BK@CNPD@Cmnd?3HealthNavigationDesc?$AA@=007dcca8

wchar_t const *HealthNavCommandClass::Get_Description() const
{
    return StringTable::Load_String("Cmnd:HealthNavigationDesc", 0, "D:\\Ra2\\Init.CPP", 6477);
}
