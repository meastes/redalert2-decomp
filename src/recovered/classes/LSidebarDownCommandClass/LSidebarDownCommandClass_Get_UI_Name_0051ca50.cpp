// wchar_t const *LSidebarDownCommandClass::Get_UI_Name() const -- 0x0051CA50
// The localized name (TXT_LSIDEBAR_DOWN).
#include "shared/command.hpp"
// @ref ?Load_String@StringTable@@SIPBGPBDPAD0H@Z=006f9ec0
// @ref ??_C@_0BA@NANB@D?3?2Ra2?2Init?4CPP?$AA@=007db8e4
// @ref ??_C@_0BC@KIIM@TXT_LSIDEBAR_DOWN?$AA@=007dd140

wchar_t const *LSidebarDownCommandClass::Get_UI_Name() const
{
    return StringTable::Load_String("TXT_LSIDEBAR_DOWN", 0, "D:\\Ra2\\Init.CPP", 7219);
}
