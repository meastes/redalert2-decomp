// wchar_t const *PageUserCommandClass::Get_UI_Name() const -- 0x0051D890
// The localized name (TXT_PAGEUSER).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0N@GDBO@TXT_PAGEUSER?$AA@=007dd618

wchar_t const *PageUserCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_PAGEUSER", 0, "D:\\Ra2\\Init.CPP", 7368);
}
