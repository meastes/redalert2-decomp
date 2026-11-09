// wchar_t const *PrevObjectCommandClass::Get_UI_Name() const -- 0x0051BCC0
// The localized name (TXT_PREV_OBJECT).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BA@CIJJ@TXT_PREV_OBJECT?$AA@=007dcd10

wchar_t const *PrevObjectCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_PREV_OBJECT", 0, "D:\\Ra2\\Init.CPP", 6562);
}
