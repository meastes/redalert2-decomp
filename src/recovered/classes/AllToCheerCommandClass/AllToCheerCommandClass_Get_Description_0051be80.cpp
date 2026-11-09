// wchar_t const *AllToCheerCommandClass::Get_Description() const -- 0x0051BE80
// The localized description (Cmnd:AllCheerDesc).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BC@FPAP@Cmnd?3AllCheerDesc?$AA@=007dcd88

wchar_t const *AllToCheerCommandClass::Get_Description() const
{
    return StringTable::Load_String("Cmnd:AllCheerDesc", 0, "D:\\Ra2\\Init.CPP", 6625);
}
