// wchar_t const *View3CommandClass::Get_Description() const -- 0x0051CFC0
// The localized description (TXT_VIEW_BOOKMARK3_DESC).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BI@PKFN@TXT_VIEW_BOOKMARK3_DESC?$AA@=007dd310

wchar_t const *View3CommandClass::Get_Description() const
{
    return StringTable::Load_String("TXT_VIEW_BOOKMARK3_DESC", 0, "D:\\Ra2\\Init.CPP", 7588);
}
