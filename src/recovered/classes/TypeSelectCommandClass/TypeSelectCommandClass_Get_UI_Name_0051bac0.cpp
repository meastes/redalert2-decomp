// wchar_t const *TypeSelectCommandClass::Get_UI_Name() const -- 0x0051BAC0
// The localized name (Cmnd:TypeSelect).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BA@DONC@Cmnd?3TypeSelect?$AA@=007dcc60

wchar_t const *TypeSelectCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("Cmnd:TypeSelect", 0, "D:\\Ra2\\Init.CPP", 6435);
}
