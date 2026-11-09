// wchar_t const *RSidebarUpCommandClass::Get_UI_Name() const -- 0x0051C780
// The localized name (TXT_RSIDEBAR_UP).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BA@OIEK@TXT_RSIDEBAR_UP?$AA@=007dd014

wchar_t const *RSidebarUpCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_RSIDEBAR_UP", 0, "D:\\Ra2\\Init.CPP", 7105);
}
