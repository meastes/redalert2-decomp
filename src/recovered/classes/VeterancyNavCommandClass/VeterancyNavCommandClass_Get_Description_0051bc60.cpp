// wchar_t const *VeterancyNavCommandClass::Get_Description() const -- 0x0051BC60
// The localized description (Cmnd:VetNavigationDesc).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BH@JECP@Cmnd?3VetNavigationDesc?$AA@=007dcce8

wchar_t const *VeterancyNavCommandClass::Get_Description() const
{
    return StringTable::Load_String("Cmnd:VetNavigationDesc", 0, "D:\\Ra2\\Init.CPP", 6503);
}
