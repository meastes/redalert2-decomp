// wchar_t const *SidebarPageUpCommandClass::Get_UI_Name() const -- 0x0051C810
// The localized name (TXT_SIDEBAR_PGUP).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BB@NMNI@TXT_SIDEBAR_PGUP?$AA@=007dd04c

wchar_t const *SidebarPageUpCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_SIDEBAR_PGUP", 0, "D:\\Ra2\\Init.CPP", 7131);
}
