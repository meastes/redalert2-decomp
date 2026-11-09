// wchar_t const *View1CommandClass::Get_Description() const -- 0x0051CDE0
// The localized description (TXT_VIEW_BOOKMARK1_DESC).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BI@NBBN@TXT_VIEW_BOOKMARK1_DESC?$AA@=007dd2a8

wchar_t const *View1CommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_VIEW_BOOKMARK1_DESC", 0, "D:\\Ra2\\Init.CPP", 7516);
}
