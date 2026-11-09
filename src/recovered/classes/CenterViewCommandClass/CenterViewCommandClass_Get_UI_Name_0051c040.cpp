// wchar_t const *CenterViewCommandClass::Get_UI_Name() const -- 0x0051C040
// The localized name (TXT_CENTER_VIEW).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BA@EFDK@TXT_CENTER_VIEW?$AA@=007dce3c

wchar_t const *CenterViewCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_CENTER_VIEW", 0, "D:\\Ra2\\Init.CPP", 6732);
}
