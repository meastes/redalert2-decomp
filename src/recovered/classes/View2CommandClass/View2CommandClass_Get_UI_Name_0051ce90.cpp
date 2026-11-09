// wchar_t const *View2CommandClass::Get_UI_Name() const -- 0x0051CE90
// The localized name (TXT_VIEW_BOOKMARK2).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BD@JHJ@TXT_VIEW_BOOKMARK2?$AA@=007dd2c8

wchar_t const *View2CommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_VIEW_BOOKMARK2", 0, "D:\\Ra2\\Init.CPP", 7548);
}
