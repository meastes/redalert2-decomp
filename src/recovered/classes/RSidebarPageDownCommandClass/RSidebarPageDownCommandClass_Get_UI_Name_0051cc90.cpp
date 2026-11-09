// wchar_t const *RSidebarPageDownCommandClass::Get_UI_Name() const -- 0x0051CC90
// The localized name (TXT_RSIDEBAR_PGDN).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BC@NDNL@TXT_RSIDEBAR_PGDN?$AA@=007dd240

wchar_t const *RSidebarPageDownCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_RSIDEBAR_PGDN", 0, "D:\\Ra2\\Init.CPP", 7308);
}
