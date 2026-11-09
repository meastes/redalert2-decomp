// wchar_t const *View4CommandClass::Get_UI_Name() const -- 0x0051D070
// The localized name (TXT_VIEW_BOOKMARK4).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BD@PGAL@TXT_VIEW_BOOKMARK4?$AA@=007dd330

wchar_t const *View4CommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_VIEW_BOOKMARK4", 0, "D:\\Ra2\\Init.CPP", 7620);
}
