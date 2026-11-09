// wchar_t const *RSidebarDownCommandClass::Get_UI_Name() const -- 0x0051CAE0
// The localized name (TXT_RSIDEBAR_DOWN).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BC@NCFB@TXT_RSIDEBAR_DOWN?$AA@=007dd180

wchar_t const *RSidebarDownCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_RSIDEBAR_DOWN", 0, "D:\\Ra2\\Init.CPP", 7246);
}
