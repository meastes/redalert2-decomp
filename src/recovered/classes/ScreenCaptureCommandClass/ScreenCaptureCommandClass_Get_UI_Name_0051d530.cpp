// wchar_t const *ScreenCaptureCommandClass::Get_UI_Name() const -- 0x0051D530
// The localized name (TXT_SCRNCAP).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0M@JMDG@TXT_SCRNCAP?$AA@=007dd468

wchar_t const *ScreenCaptureCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_SCRNCAP", 0, "D:\\Ra2\\Init.CPP", 7877);
}
