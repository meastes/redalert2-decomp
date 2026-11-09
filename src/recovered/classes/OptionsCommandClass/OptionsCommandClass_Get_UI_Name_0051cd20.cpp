// wchar_t const *OptionsCommandClass::Get_UI_Name() const -- 0x0051CD20
// The localized name (TXT_OPTIONS).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0M@JGED@TXT_OPTIONS?$AA@=007dd26c

wchar_t const *OptionsCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_OPTIONS", 0, "D:\\Ra2\\Init.CPP", 7335);
}
