// wchar_t const *SidebarPageDownCommandClass::Get_UI_Name() const -- 0x0051CB70
// The localized name (TXT_SIDEBAR_PGDN).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BB@KCGB@TXT_SIDEBAR_PGDN?$AA@=007dd1bc

wchar_t const *SidebarPageDownCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_SIDEBAR_PGDN", 0, "D:\\Ra2\\Init.CPP", 7272);
}
