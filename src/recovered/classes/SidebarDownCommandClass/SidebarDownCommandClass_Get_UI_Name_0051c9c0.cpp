// wchar_t const *SidebarDownCommandClass::Get_UI_Name() const -- 0x0051C9C0
// The localized name (TXT_SIDEBAR_DOWN).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BB@KDOL@TXT_SIDEBAR_DOWN?$AA@=007dd104

wchar_t const *SidebarDownCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_SIDEBAR_DOWN", 0, "D:\\Ra2\\Init.CPP", 7193);
}
