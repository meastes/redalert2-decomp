// wchar_t const *ScreenCaptureCommandClass::Get_Description() const -- 0x0051D550
// The localized description (TXT_SCRNCAP_DESC).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BB@PDJF@TXT_SCRNCAP_DESC?$AA@=007dd474

wchar_t const *ScreenCaptureCommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_SCRNCAP_DESC", 0, "D:\\Ra2\\Init.CPP", 7878);
}
