// wchar_t const *AllToCheerCommandClass::Get_Category() const -- 0x0051BE60
// The localized category (TXT_CONTROL).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0M@NOFB@TXT_CONTROL?$AA@=007dcbec

wchar_t const *AllToCheerCommandClass::Get_Category() const
{
    return StringTable::Load_String("TXT_CONTROL", 0, "D:\\Ra2\\Init.CPP", 6623);
}
